#include "pch.h"
#include "pool_impl.h"
#include <curl/curl.h>

using namespace sinet;

refptr<pool> pool::create_instance()
{
  refptr<pool> _pool(new pool_impl());
  return _pool;
}

pool_impl::pool_impl(void):
  m_curl(curl_multi_init())
{

}

pool_impl::~pool_impl(void)
{
  curl_multi_cleanup(m_curl);
}

void pool_impl::execute(refptr<task> task)
{

}

void pool_impl::cancel(refptr<task> task)
{

}