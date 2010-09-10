#include "pch.h"
#include "task_impl.h"
#include <curl/curl.h>

using namespace sinet;

refptr<task> task::create_instance()
{
  refptr<task> _task(new task_impl());
  return _task;
}

task_impl::task_impl(void)
{
}

task_impl::~task_impl(void)
{
}

int task_impl::append_request(refptr<request> request_in)
{
  return -1;
}

int task_impl::erase_request(int request_id)
{
  return -1;
}

void task_impl::clearall_requests()
{

}

int task_impl::get_request_count()
{
  return -1;
}

std::vector<int> task_impl::get_request_ids()
{
  return std::vector<int>();
}

refptr<request> task_impl::get_request(int request_id)
{
  return refptr<request>();
}

void task_impl::set_status(int status)
{

}

int task_impl::get_status()
{
  return -1;
}

void task_impl::attach_observer(itask_observer* observer_in)
{

}

void task_impl::detach_observer()
{

}