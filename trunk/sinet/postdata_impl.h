#ifndef POSTDATA_IMPL_H
#define POSTDATA_IMPL_H

#include "postdata.h"

namespace sinet
{

class postdata_impl:
  public threadsafe_base<postdata>
{
public:
  virtual void clear();
  virtual int add_elem(refptr<postdataelem> elem);
  virtual int remove_elem(refptr<postdataelem> elem);
  virtual int get_elements(std::vector<refptr<postdataelem> >& elems);
  virtual int get_element_count();
};

}

#endif // POSTDATA_IMPL_H