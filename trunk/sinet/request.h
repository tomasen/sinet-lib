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

};

} // namespace sinet

#endif // SINET_REQUEST_H