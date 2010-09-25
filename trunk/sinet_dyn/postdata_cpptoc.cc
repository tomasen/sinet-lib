#include "pch.h"
#include "postdata_cpptoc.h"

using namespace sinet;

SINET_DYN_API _postdata_t* _postdata_create_instance()
{
  refptr<postdata> impl = postdata::create_instance();
  if (impl.get())
    return postdata_cpptoc::Wrap(impl);
  return NULL;
}

void SINET_DYN_CALLBACK _clear(struct __postdata_t* self)
{
}

void SINET_DYN_CALLBACK _add_elem(struct __postdata_t* self, _postdataelem_t elem)
{

}

int SINET_DYN_CALLBACK _remove_elem(struct __postdata_t* self, _postdataelem_t elem)
{
  return 0;
}

void SINET_DYN_CALLBACK _get_elements(struct __postdata_t* self, _postdataelem_t* elems)
{

}

int SINET_DYN_CALLBACK _get_element_count(struct __postdata_t* self)
{
  return 0;
}


postdata_cpptoc::postdata_cpptoc(postdata* cls):
cpptoc<postdata_cpptoc, postdata, _postdata_t>(cls)
{

}