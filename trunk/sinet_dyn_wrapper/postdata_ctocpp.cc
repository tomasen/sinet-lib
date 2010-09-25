#include "pch.h"
#include "postdata_ctocpp.h"

using namespace sinet;

refptr<postdata> postdata::create_instance()
{
  _postdata_t* impl = _postdata_create_instance();
  if (impl)
    return postdata_ctocpp::Wrap(impl);
  return NULL;
}

void postdata_ctocpp::clear()
{

}

void postdata_ctocpp::add_elem(refptr<postdataelem> elem)
{

}

int postdata_ctocpp::remove_elem(refptr<postdataelem> elem)
{
  return 0;
}

void postdata_ctocpp::get_elements(std::vector<refptr<postdataelem> >& elems)
{

}

int postdata_ctocpp::get_element_count()
{
  return 0;
}