#ifndef SINET_REQUEST_IMPL_H
#define SINET_REQUEST_IMPL_H

#include "request.h"

namespace sinet
{

class request_impl:
  public threadsafe_base<request>
{
public:
  request_impl(void);
  ~request_impl(void);

  virtual void set_request_method(std::wstring& method);
  virtual std::wstring get_request_method();

  virtual void set_request_url(std::wstring& method);
  virtual std::wstring get_request_url();

  virtual void set_request_header(si_stringmap& header);
  virtual si_stringmap get_request_header();

  virtual void set_postdata(refptr<postdata> postdata);
  virtual refptr<postdata> get_postdata();

  virtual void set_response_header(si_stringmap& header);
  virtual si_stringmap get_response_header();

  virtual void set_response_buffer(si_buffer& buffer);
  virtual si_buffer get_response_buffer();

  virtual void set_response_size(size_t size_in);
  virtual size_t get_response_size();

  virtual void set_response_errcode(int errcode);
  virtual int get_response_errcode();

private:
  size_t m_response_size;
};

} // namespace sinet

#endif // SINET_REQUEST_IMPL_H