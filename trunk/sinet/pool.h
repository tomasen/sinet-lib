#ifndef SINET_POOL_H
#define SINET_POOL_H

#include "api_base.h"
#include "api_refptr.h"

namespace sinet
{

class pool:
  public base
{
public:
  static refptr<pool> create_instance();

};

} // namespace sinet

#endif // SINET_POOL_H