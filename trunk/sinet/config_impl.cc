#include "pch.h"
#include "config_impl.h"

using namespace sinet;

refptr<config> config::create_instance()
{
  refptr<config> _config(new config_impl());
  return _config;
}
