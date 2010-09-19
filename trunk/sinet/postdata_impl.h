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
  virtual void add_elem(refptr<postdataelem> elem);
  virtual int remove_elem(refptr<postdataelem> elem);
  virtual void get_elements(std::vector<refptr<postdataelem> >& elems);
  virtual int get_element_count();
  virtual std::wstring get_form_name();
  virtual void set_form_name(std::wstring formname);

private:
  std::vector<refptr<postdataelem> > m_elems;
  std::wstring                       m_form_name;
};

}

#endif // POSTDATA_IMPL_H