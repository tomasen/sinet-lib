#include "pch.h"
#include "postdata_impl.h"

using namespace sinet;

refptr<postdata> postdata::create_instance()
{
  refptr<postdata> _postdata(new postdata_impl());
  return _postdata;
}

void postdata_impl::clear()
{

}

int postdata_impl::add_elem(refptr<postdataelem> elem)
{
  return 0;
}

int postdata_impl::remove_elem(refptr<postdataelem> elem)
{
  return 0;
}

int postdata_impl::get_elements(std::vector<refptr<postdataelem> >& elems)
{
  return 0;
}

int postdata_impl::get_element_count()
{
  return 0;
}