#include "pch.h"
#include "pool_impl.h"

using namespace sinet;

refptr<pool> pool::create_instance()
{
  refptr<pool> _pool(new pool_impl());
  return _pool;
}
