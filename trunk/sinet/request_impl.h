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

  virtual void set_response_size(size_t size_in);
  virtual size_t get_response_size();

private:
  size_t m_response_size;
};

} // namespace sinet

#endif // SINET_REQUEST_IMPL_H