#ifndef SINET_POOL_IMPL_H
#define SINET_POOL_IMPL_H

#include "pool.h"

typedef void CURLM;
typedef void CURL;

namespace sinet
{

class pool_impl:
  public threadsafe_base<pool>
{
public:
  pool_impl(void);
  ~pool_impl(void);

  virtual void execute(refptr<task> task);
  virtual void cancel(refptr<task> task);

private:
  CURLM* m_curl;

  std::map<CURL*, refptr<task> >  m_tasks;
};

} // namespace sinet

#endif // SINET_POOL_IMPL_H