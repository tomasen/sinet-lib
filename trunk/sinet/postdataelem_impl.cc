#include "pch.h"
#include "postdataelem_impl.h"

using namespace sinet;

refptr<postdataelem> postdataelem::create_instance()
{
  refptr<postdataelem> _postdataelem(new postdataelem_impl());
  return _postdataelem;
}

void postdataelem_impl::set_name(const wchar_t* fieldname)
{

}

std::wstring postdataelem_impl::get_name()
{
  return L"";
}

void postdataelem_impl::setto_empty()
{

}

void postdataelem_impl::setto_file(const wchar_t* filename)
{

}

void postdataelem_impl::setto_buffer(const void* bytes_in, const size_t size_in)
{

}

void postdataelem_impl::setto_text(const wchar_t* text)
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

std::wstring postdataelem_impl::get_text()
{
  return L"";
}