#ifndef SINET_TASK_H
#define SINET_TASK_H

#include "api_base.h"
#include "api_refptr.h"

namespace sinet
{

class task:
  public base
{
public:
  static refptr<task> create_instance();

};

} // namespace sinet


#endif // SINET_TASK_H