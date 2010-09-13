#ifndef POSTDATAELEM_IMPL_H
#define POSTDATAELEM_IMPL_H

#include "postdataelem.h"

namespace sinet
{

class postdataelem_impl:
  public threadsafe_base<postdataelem>
{
public:
  virtual void setto_empty();
  virtual void setto_file(const std::wstring& filename);
  virtual void setto_buffer(const void* bytes_in, const size_t size_in);

  virtual postdataelem_type_t get_type();

  virtual std::wstring get_file();
  virtual size_t get_buffer_size();
  virtual size_t copy_buffer_to(const void* bytes_inout, size_t size_in);
};

} // namespace sinet

#endif // POSTDATAELEM_IMPL_H