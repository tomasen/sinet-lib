#include "pch.h"
#include "request_impl.h"

using namespace sinet;

refptr<request> request::create_instance()
{
  refptr<request> _request(new request_impl());
  return _request;
}
