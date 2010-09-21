#include "pch.h"
#include "postdataelem_cpptoc.h"

using namespace sinet;

SINET_DYN_API _postdataelem_t* _postdataelem_create_instance()
{
  refptr<postdataelem> impl = postdataelem::create_instance();
  if (impl.get())
    return postdataelem_cpptoc::Wrap(impl);
  return NULL;
}

void SINET_DYN_CALLBACK _set_name(struct __postdataelem_t* self, const wchar_t* fieldname)
{

}

_string_t SINET_DYN_CALLBACK _get_name(struct __postdataelem_t* self)
{

}

void SINET_DYN_CALLBACK _setto_empty(struct __postdataelem_t* self)
{

}

void SINET_DYN_CALLBACK _setto_file(struct __postdataelem_t* self, const wchar_t* filename)
{

}

void SINET_DYN_CALLBACK _setto_buffer(struct __postdataelem_t* self, const void* bytes_in, const size_t size_in)
{

}

void SINET_DYN_CALLBACK _setto_text(struct __postdataelem_t* self, const wchar_t* text)
{

}

postdataelem_type_t SINET_DYN_CALLBACK _get_type(struct __postdataelem_t* self)
{

}

_string_t SINET_DYN_CALLBACK _get_file(struct __postdataelem_t* self)
{

}

size_t SINET_DYN_CALLBACK _get_buffer_size(struct __postdataelem_t* self)
{

}

size_t SINET_DYN_CALLBACK _copy_buffer_to(struct __postdataelem_t* self, void* bytes_inout, size_t size_in)
{

}

_string_t SINET_DYN_CALLBACK _get_text(struct __postdataelem_t* self)
{

}

postdataelem_cpptoc::postdataelem_cpptoc(postdataelem* cls) :
  cpptoc<postdataelem_cpptoc, postdataelem, _postdataelem_t>(cls)
{

}