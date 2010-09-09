#ifndef API_BASE_H
#define API_BASE_H

namespace sinet
{

class base
{
public:
  virtual int AddRef() =0;
  virtual int Release() =0;
  virtual int GetRefCt() = 0;
};

#ifdef _WINDOWS_

#define atomic_increment(p) InterlockedIncrement(p)
#define atomic_decrement(p) InterlockedDecrement(p)

class critical_section
{
public:
  critical_section()
  {
    memset(&m_sec, 0, sizeof(CRITICAL_SECTION));
    InitializeCriticalSection(&m_sec);
  }
  ~critical_section()
  {
    DeleteCriticalSection(&m_sec);
  }
  void Lock()
  {
    EnterCriticalSection(&m_sec);
  }
  void Unlock()
  {
    LeaveCriticalSection(&m_sec);
  }
  CRITICAL_SECTION m_sec;
};

#endif // _WINDOWS_

template <class ClassName>
class threadsafe_base : public ClassName
{
public:
  threadsafe_base()
  {
    m_dwRef = 0L;
  }
  virtual ~threadsafe_base()
  {
  }

  virtual int AddRef()
  {
    return atomic_increment(&m_dwRef);
  }

  virtual int Release()
  {
    int retval = atomic_decrement(&m_dwRef);
    if(retval == 0)
      delete this;
    return retval;
  }

  virtual int GetRefCt() { return m_dwRef; }

  void Lock() { m_critsec.Lock(); }
  void Unlock() { m_critsec.Unlock(); }

protected:
  long m_dwRef;
  critical_section m_critsec;
};

} // namespace sinet

#endif // API_BASE_H