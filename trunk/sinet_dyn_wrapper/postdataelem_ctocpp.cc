#include "pch.h"
#include "postdataelem_ctocpp.h"

using namespace sinet;

refptr<postdataelem> postdataelem::create_instance()
{
  _postdataelem_t* impl = _postdataelem_create_instance();
  if (impl)
    return postdataelem_ctocpp::Wrap(impl);
  return NULL;
}

void postdataelem_ctocpp::set_name( const wchar_t* fieldname )
{

}

std::wstring postdataelem_ctocpp::get_name()
{
  return L"";
}

void postdataelem_ctocpp::setto_empty()
{

}

void postdataelem_ctocpp::setto_file( const wchar_t* filename )
{

}

void postdataelem_ctocpp::setto_buffer( const void* bytes_in, const size_t size_in )
{

}

void postdataelem_ctocpp::setto_text( const wchar_t* text )
{

}

sinet::postdataelem_type_t postdataelem_ctocpp::get_type()
{
  return PDE_TYPE_EMPTY;
}

std::wstring postdataelem_ctocpp::get_file()
{
  return L"";
}

size_t postdataelem_ctocpp::get_buffer_size()
{
  return 0;
}

size_t postdataelem_ctocpp::copy_buffer_to( void* bytes_inout, size_t size_in )
{
  return 0;
}

std::wstring postdataelem_ctocpp::get_text()
{
  return L"";
}