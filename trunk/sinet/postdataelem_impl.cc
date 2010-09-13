#include "pch.h"
#include "postdataelem_impl.h"

using namespace sinet;

refptr<postdataelem> postdataelem::create_instance()
{
  refptr<postdataelem> _postdataelem(new postdataelem_impl());
  return _postdataelem;
}

void postdataelem_impl::setto_empty()
{

}

void postdataelem_impl::setto_file(const std::wstring& filename)
{

}

void postdataelem_impl::setto_buffer(const void* bytes_in, const size_t size_in)
{

}

postdataelem_type_t postdataelem_impl::get_type()
{
  return PDE_TYPE_EMPTY;
}

std::wstring postdataelem_impl::get_file()
{
  return L"";
}

size_t postdataelem_impl::get_buffer_size()
{
  return 0;
}

size_t postdataelem_impl::copy_buffer_to(const void* bytes_inout, size_t size_in)
{
  return 0;
}