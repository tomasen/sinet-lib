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

void request_impl::set_request_method( std::wstring& method )
{

}

std::wstring request_impl::get_request_method()
{
  return L"";
}

void request_impl::set_request_url( std::wstring& method )
{

}

std::wstring request_impl::get_request_url()
{
  return L"";
}

void request_impl::set_request_header( si_stringmap& header )
{

}

si_stringmap request_impl::get_request_header()
{
  return si_stringmap();
}

void request_impl::set_postdata( refptr<postdata> postdata )
{

}

refptr<postdata> request_impl::get_postdata()
{
  return NULL;
}

void request_impl::set_response_header( si_stringmap& header )
{

}

si_stringmap request_impl::get_response_header()
{
  return si_stringmap();
}

void request_impl::set_response_buffer( si_buffer& buffer )
{

}

si_buffer request_impl::get_response_buffer()
{
  return si_buffer();
}

void request_impl::set_response_size(size_t size_in)
{
  m_response_size = size_in;
}

size_t request_impl::get_response_size()
{
  return m_response_size;
}

void request_impl::set_response_errcode( int errcode )
{

}

int request_impl::get_response_errcode()
{
  return 0;
}