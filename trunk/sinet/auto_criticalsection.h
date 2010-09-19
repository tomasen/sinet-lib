#ifndef API_auto_criticalsection_H
#define API_auto_criticalsection_H

#include "pch.h"
#include "api_base.h"

namespace sinet
{

class auto_criticalsection
{
public:
  auto_criticalsection(critical_section& cs): m_cs(&cs)
  {
    m_cs->lock();
  }
  virtual ~auto_criticalsection()
  {
    m_cs->unlock();
  }
private:
  critical_section* m_cs;
};

}

#endif // API_auto_criticalsection_H