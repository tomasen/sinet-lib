#ifndef SINET_TASK_IMPL_H
#define SINET_TASK_IMPL_H

#include "task.h"

namespace sinet
{

typedef void CURL;

class task_impl:
  public threadsafe_base<task>
{
public:
  task_impl(void);
  ~task_impl(void);

private:
  CURL* m_curl;
};

} // namespace sine t

#endif // SINET_TASK_IMPL_H