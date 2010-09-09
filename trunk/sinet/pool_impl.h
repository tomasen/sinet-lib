#ifndef SINET_POOL_IMPL_H
#define SINET_POOL_IMPL_H

#include "pool.h"

namespace sinet
{

class pool_impl:
  public threadsafe_base<pool>
{
public:
};

} // namespace sinet

#endif // SINET_POOL_IMPL_H