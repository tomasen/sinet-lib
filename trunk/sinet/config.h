#ifndef SINET_CONFIG_H
#define SINET_CONFIG_H

#include "api_base.h"
#include "api_refptr.h"

namespace sinet
{

class config:
  public base
{
public:
  static refptr<config> create_instance();

};

} // namespace sinet

#endif // SINET_CONFIG_H