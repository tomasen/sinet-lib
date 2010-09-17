#include "pch.h"
#include "config_impl.h"

using namespace sinet;

refptr<config> config::create_instance()
{
  refptr<config> _config(new config_impl());
  return _config;
}


int config_impl::get_strvar(int id, std::wstring& strvarout)
{
  std::map<int, std::wstring>::iterator it = m_strvar.find(id);
  if (it != m_strvar.end())
  {
    strvarout = it->second;
    return 1;
  }
  return 0;
}

void config_impl::set_strvar(int id, std::wstring strvarin)
{
  m_csconfig.lock();
  m_strvar[id] = strvarin;
  m_csconfig.unlock();
}

int config_impl::remove_strvar(int id)
{
  std::map<int, std::wstring>::iterator it = m_strvar.find(id);
  if (it != m_strvar.end())
  {
    m_csconfig.lock();
    m_strvar.erase(it);
    m_csconfig.unlock();
    return 1;
  }
  return 0;
}