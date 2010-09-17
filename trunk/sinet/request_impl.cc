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

void request_impl::set_request_method(const wchar_t* method)
{
  m_method = method;
}

std::wstring request_impl::get_request_method()
{
  return m_method;
}

void request_impl::set_request_url(const wchar_t* url)
{
  m_url = url;
}

std::wstring request_impl::get_request_url()
{
  return m_url;
}

void request_impl::set_request_header(si_stringmap& header)
{
  m_header = header;
}

si_stringmap request_impl::get_request_header()
{
  return m_header;
}

void request_impl::set_postdata(refptr<postdata> postdata)
{
  m_postdata = postdata;
}

refptr<postdata> request_impl::get_postdata()
{
  return m_postdata;
}

void request_impl::set_response_header(si_stringmap& header)
{
  m_response_header = header;
}

si_stringmap request_impl::get_response_header()
{
  return m_response_header;
}

void request_impl::set_response_buffer(si_buffer& buffer)
{
  size_t rsz = min(buffer.size(), m_response_size);
  if (rsz == 0)
    return;
  m_response_buffer.resize(rsz);
  memcpy(&m_response_buffer[0], &buffer[0], rsz);
}

si_buffer request_impl::get_response_buffer()
{
  return m_response_buffer;
}

void request_impl::set_response_size(size_t size_in)
{
  m_response_size = size_in;
}

size_t request_impl::get_response_size()
{
  return m_response_size;
}

void request_impl::set_response_errcode(int errcode)
{
  m_response_errcode = errcode;
}

int request_impl::get_response_errcode()
{
  return m_response_errcode;
}