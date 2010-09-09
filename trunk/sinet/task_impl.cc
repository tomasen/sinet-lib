#include "pch.h"
#include "task_impl.h"
#include <curl/curl.h>

using namespace sinet;

refptr<task> task::create_instance()
{
  refptr<task> _task(new task_impl());
  return _task;
}

sinet::task_impl::task_impl(void):
  m_curl(curl_easy_init())
{

}

sinet::task_impl::~task_impl(void)
{
  curl_easy_cleanup(m_curl);
}