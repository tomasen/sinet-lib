#include "pch.h"
#include "request_impl.h"

using namespace sinet;

refptr<request> request::create_instance()
{
  refptr<request> _request(new request_impl());
  return _request;
}

request_impl::request_impl(void):
  m_response_size(0)
{

}

request_impl::~request_impl(void)
{

}

void request_impl::set_response_size(size_t size_in)
{
  m_response_size = size_in;
}

size_t request_impl::get_response_size()
{
  return m_response_size;
}

