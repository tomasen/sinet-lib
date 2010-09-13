#include "pch.h"
#include "pool_impl.h"
#include <curl/curl.h>
#include <process.h>

using namespace sinet;

refptr<pool> pool::create_instance()
{
  refptr<pool> _pool(new pool_impl());
  return _pool;
}

static size_t write_mem_callback(void* ptr, size_t size, size_t nmemb, void* data)
{
  size_t realsize = size * nmemb;
  refptr<request> request_in((request*)data);
  request_in->set_response_size(request_in->get_response_size() + realsize + 1);
  return realsize;
}

pool_impl::pool_impl(void)
{
  m_stop_event = ::CreateEvent(NULL, TRUE, FALSE, NULL);
  m_thread = (HANDLE)::_beginthread(_thread_dispatch, 0, (void*)this);
}

pool_impl::~pool_impl(void)
{
  _stop_thread();
  clear_all();
  ::CloseHandle(m_stop_event);
  // when a thread created by _beginthread is gracefully closed, it'll
  // call CloseHandle automatically, so there's no need for additional
  // free up operation.
}

void pool_impl::execute(refptr<task> task_in)
{
  if (!task_in)
    return;

  task_in->set_status(taskstatus_queued);

  m_cstask_queue.lock();
  m_task_queue.push_back(task_in);
  m_cstask_queue.unlock();
}

void pool_impl::cancel(refptr<task> task_in)
{
  // stop task if it's running
  m_cstasks_running.lock();
  std::map<refptr<task>, task_info>::iterator it = m_tasks_running.find(task_in);
  if (it != m_tasks_running.end())
  {
    _cancel_running_task(it->second.hmaster, it->second.htasks);
    it->first->set_status(taskstatus_canceled);
    m_cstask_finished.lock();
    m_task_finished.push_back(it->first);
    m_cstask_finished.unlock();
    m_tasks_running.erase(it);
  }
  m_cstasks_running.unlock();
  // clear task if it's queued
  m_cstask_queue.lock();
  for (std::vector<refptr<task> >::iterator it = m_task_queue.begin();
    it != m_task_queue.end(); it++)
  {
    if (*it == task_in)
      m_task_queue.erase(it);
  }
  m_cstask_queue.unlock();
}

int pool_impl::is_running(refptr<task> task_in)
{
  int ret = 0;
  m_cstasks_running.lock();
  std::map<refptr<task>, task_info>::iterator it = m_tasks_running.find(task_in);
  if (it != m_tasks_running.end() && it->second.running_handle > 0)
    ret = 1;
  m_cstasks_running.unlock();
  return ret;
}

int pool_impl::is_queued(refptr<task> task_in)
{
  int ret = 0;
  m_cstask_queue.lock();
  for (std::vector<refptr<task> >::iterator it = m_task_queue.begin();
    it != m_task_queue.end(); it++)
  {
    if (*it == task_in)
    {
      ret = 1;
      break;
    }
  }
  m_cstask_queue.unlock();
  return ret;
}

int pool_impl::is_running_or_queued(refptr<task> task_in)
{
  int ret = 0;
  m_cstasks_running.lock();
  m_cstask_queue.lock();
  std::map<refptr<task>, task_info>::iterator it = m_tasks_running.find(task_in);
  if (it != m_tasks_running.end() && it->second.running_handle > 0)
    ret = 1;
  for (std::vector<refptr<task> >::iterator it = m_task_queue.begin();
    it != m_task_queue.end(); it++)
  {
    if (*it == task_in)
    {
      ret = 1;
      break;
    }
  }
  m_cstask_queue.unlock();
  m_cstasks_running.unlock();
  return ret;
}

void pool_impl::clear_all()
{
  // clear running tasks
  m_cstasks_running.lock();
  for (std::map<refptr<task>, task_info>::iterator it = m_tasks_running.begin();
    it != m_tasks_running.end(); it++)
    _cancel_running_task(it->second.hmaster, it->second.htasks);
  m_tasks_running.clear();
  m_cstasks_running.unlock();
  // clear tasks in queue
  m_cstask_queue.lock();
  m_task_queue.resize(0);
  m_cstask_queue.unlock();
  // clear finished tasks
  m_cstask_finished.lock();
  m_task_finished.resize(0);
  m_cstask_finished.unlock();
}

void pool_impl::_thread_dispatch(void* param)
{
  static_cast<pool_impl*>(param)->_thread();
}

void pool_impl::_thread()
{
  // this is is the master pool thread, controlling pool workflow
  // to enable task executing and stopping

  // the while loop only exists when m_stop_event is signaled by
  // _stop_thread()
  while (::WaitForSingleObject(m_stop_event, 5) == WAIT_TIMEOUT)
  {
    // thread loop procedure is as follows:
    // 1. iterate through |m_tasks_running| and try curl_multi_perform
    //    on all of them, if one's |ti.running_handles| equals 0, move
    //    it to |m_tasks_finished|
    // 2. if all tasks' |ti.running_handles| in |m_tasks_running|
    //    equals 0, go to step 3, otherwise, go back to beginning
    // 3. check |m_tasks_queued| to see if there are anything left
    //    should be prepared for running

    // step 1.
    int tasks_still_running = 0;
    std::vector<std::map<refptr<task>, task_info>::iterator > its_running_toclear;

    m_cstasks_running.lock();
    for (std::map<refptr<task>, task_info>::iterator it = m_tasks_running.begin();
      it != m_tasks_running.end(); it++)
    {
      ::curl_multi_perform(it->second.hmaster, &it->second.running_handle);
      if (it->second.running_handle == 0)
      {
        its_running_toclear.push_back(it);  // prepare to remove
        m_cstask_finished.lock();
        m_task_finished.push_back(it->first);
        m_cstask_finished.unlock();
      }
      else
        tasks_still_running++;
    }
    for (std::vector<std::map<refptr<task>, task_info>::iterator >::iterator it = 
      its_running_toclear.begin(); it != its_running_toclear.end(); it++)
    {
      _cancel_running_task((*it)->second.hmaster, (*it)->second.htasks);
      (*it)->first->set_status(taskstatus_completed);
      m_tasks_running.erase(*it);
    }
    m_cstasks_running.unlock();

    // step 2.
    if (tasks_still_running > 0)
      continue;

    // step 3.
    m_cstasks_running.lock();
    m_cstask_queue.lock();
    std::vector<refptr<task> >::iterator it = m_task_queue.begin();
    if (it != m_task_queue.end())
    {
      task_info ti;
      _prepare_task(*it, ti);
      m_tasks_running[*it] = ti;
      ::curl_multi_perform(ti.hmaster, 
        &m_tasks_running.find(*it)->second.running_handle);
      m_task_queue.erase(it);
    }
    m_cstask_queue.unlock();
    m_cstasks_running.unlock();
  }
}

void pool_impl::_prepare_task(refptr<task> task_in, task_info& taskinfo_in_out)
{
  // things below are bogus curl calls for testing workflow
  // _prepare_task should iterator through all requests in refptr<task>
  // and make corresponding curl calls

  taskinfo_in_out.running_handle = 0;

  taskinfo_in_out.hmaster = ::curl_multi_init();
  refptr<request> req = task_in->get_request(0);
  // to do, fetch tasks
  CURL* curl = ::curl_easy_init();
  //::curl_easy_setopt(curl, CURLOPT_URL, "http://www.plu.cn/");
  //::curl_easy_setopt(curl, CURLOPT_URL, "https://www.shooter.cn/tmp/alu.jpg");
  //::curl_easy_setopt(curl, CURLOPT_URL, "http://www.shooter.cn/tmp/alu.jpg");
  ::curl_easy_setopt(curl, CURLOPT_URL, "http://dl.baofeng.com/storm3/Storm2012-3.10.09.05.exe");
  ::curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  ::curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_mem_callback);
  ::curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)req.get());
  ::curl_easy_setopt(curl, CURLOPT_PROXY, "");
  taskinfo_in_out.htasks.push_back(curl);
  ::curl_multi_add_handle(taskinfo_in_out.hmaster, curl);
}

void pool_impl::_cancel_running_task(CURLM*& hmaster, std::vector<CURL*>& htasks) 
{
  for (std::vector<CURL*>::iterator it = htasks.begin(); it != htasks.end(); it++)
  {
    ::curl_multi_remove_handle(hmaster, *it);
    ::curl_easy_cleanup(*it);
  }
  ::curl_multi_cleanup(hmaster);
}

void sinet::pool_impl::_stop_thread()
{
  ::SetEvent(m_stop_event);
  ::WaitForSingleObject(m_thread, INFINITE);
}