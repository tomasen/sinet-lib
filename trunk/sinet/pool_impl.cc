#include "pch.h"
#include "pool_impl.h"
#include <curl/curl.h>

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

}

pool_impl::~pool_impl(void)
{
  clear_all();
}

void pool_impl::execute(refptr<task> task_in)
{
  if (!task_in)
    return;

  task_in->set_status(taskstatus_running);

  task_info ti;
  _prepare_task(task_in, ti);
  m_cstasks.Lock();
  m_tasks[task_in] = ti;
  ::curl_multi_perform(ti.hmaster, &m_tasks.find(task_in)->second.running_handle);
  m_cstasks.Unlock();
}

void pool_impl::cancel(refptr<task> task_in)
{
  m_cstasks.Lock();
  std::map<refptr<task>, task_info>::iterator it = m_tasks.find(task_in);
  if (it == m_tasks.end())
    return;
  _clean_task(it->second.hmaster, it->second.htasks);
  m_cstasks.Unlock();
}

int pool_impl::is_running(refptr<task> task_in)
{
  std::map<refptr<task>, task_info>::iterator it = m_tasks.find(task_in);
  if (it == m_tasks.end())
    return 0;
  ::curl_multi_perform(it->second.hmaster, &it->second.running_handle);
  if (it->second.running_handle > 0)
    return 1;
  return 0;
}

void pool_impl::clear_all()
{
  m_cstasks.Lock();
  for (std::map<refptr<task>, task_info>::iterator it = m_tasks.begin();
    it != m_tasks.end(); it++)
    _clean_task(it->second.hmaster, it->second.htasks);
  m_tasks.clear();
  m_cstasks.Unlock();
}

void pool_impl::clean_finished()
{
  m_cstasks.Lock();
  for (std::map<refptr<task>, task_info>::iterator it = m_tasks.begin();
    it != m_tasks.end(); it++)
  {
    if (it->first->get_status() > taskstatus_running)
      _clean_task(it->second.hmaster, it->second.htasks);
  }
  m_cstasks.Unlock();
}

void pool_impl::_prepare_task(refptr<task> task_in, task_info& taskinfo_in)
{
  taskinfo_in.hmaster = ::curl_multi_init();
  refptr<request> req = task_in->get_request(0);
  // to do, fetch tasks
  CURL* curl = ::curl_easy_init();
  //::curl_easy_setopt(curl, CURLOPT_URL, "http://www.plu.cn/");
  ::curl_easy_setopt(curl, CURLOPT_URL, "https://www.shooter.cn/tmp/alu.jpg");
  //::curl_easy_setopt(curl, CURLOPT_URL, "http://www.shooter.cn/tmp/alu.jpg");
  ::curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  ::curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_mem_callback);
  ::curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)req.get());
  ::curl_easy_setopt(curl, CURLOPT_PROXY, "");
  taskinfo_in.htasks.push_back(curl);
  ::curl_multi_add_handle(taskinfo_in.hmaster, curl);
}

void pool_impl::_clean_task(CURLM*& hmaster, std::vector<CURL*>& htasks) 
{
  for (std::vector<CURL*>::iterator it = htasks.begin(); it != htasks.end(); it++)
  {
    ::curl_multi_remove_handle(hmaster, *it);
    ::curl_easy_cleanup(*it);
  }
  ::curl_multi_cleanup(hmaster);
}
