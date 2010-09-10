#ifndef SINET_REQUEST_H
#define SINET_REQUEST_H

#include "api_base.h"
#include "api_refptr.h"

namespace sinet
{

class request:
  public base
{
public:
  static refptr<request> create_instance();

  virtual void set_response_size(size_t size_in) = 0;
  virtual size_t get_response_size() = 0;
};

} // namespace sinet

#endif // SINET_REQUEST_H