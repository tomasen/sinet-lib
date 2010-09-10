#ifndef SINET_POOL_IMPL_H
#define SINET_POOL_IMPL_H

#include "pool.h"

typedef void CURLM;
typedef void CURL;

namespace sinet
{

class pool_impl:
  public threadsafe_base<pool>
{
public:
  pool_impl(void);
  ~pool_impl(void);

  virtual void execute(refptr<task> task_in);
  virtual void cancel(refptr<task> task_in);
  virtual int is_running(refptr<task> task_in);
  virtual void clear_all();
  virtual void clean_finished();

  typedef struct _task_info{
    CURLM* hmaster;
    std::vector<CURL*> htasks;
    int running_handle;
  }task_info;

private:
  void _prepare_task(refptr<task> task_in, task_info& taskinfo_in);
  void _clean_task(CURLM*& hmaster, std::vector<CURL*>& htasks);

  critical_section                  m_cstasks;
  std::map<refptr<task>, task_info> m_tasks;
};

} // namespace sinet

#endif // SINET_POOL_IMPL_H