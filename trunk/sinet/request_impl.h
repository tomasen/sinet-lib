#ifndef SINET_REQUEST_IMPL_H
#define SINET_REQUEST_IMPL_H

#include "request.h"

namespace sinet
{

class request_impl:
  public threadsafe_base<request>
{
public:
};

} // namespace sinet

#endif // SINET_REQUEST_IMPL_H