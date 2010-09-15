#include "pch.h"
#include "../../sinet/sinet.h"
#include <time.h>

using namespace sinet;

#define SINET_VALIDATE_INT(test_name, condition, variable) \
  { \
    if (##condition)\
      printf ("test [%s] is [OK]\n", test_name);\
    else\
      printf ("test [%s] [FAILED], condition requires (" #condition ") and its value is: %d\n", test_name, variable);\
    \
  }

#define SINET_VALIDATE_INT64(test_name, condition, variable) \
{ \
  if (##condition)\
  printf ("test [%s] is [OK]\n", test_name);\
    else\
    printf ("test [%s] [FAILED], condition requires (" #condition ") and its value is: %lld\n", test_name, variable);\
    \
}


void run_test_poolcreation()
{
  printf("--------------------------------------------\n");
  printf("run_test_poolcreation\n");
  int num_pools = 1000;
  printf("start timing for %d pool creation ...\n", num_pools);
  //size_t clock_begin = clock();
  LARGE_INTEGER clock_begin;
  ::QueryPerformanceCounter(&clock_begin);
  std::vector<refptr<pool> > pools(num_pools);
  for (std::vector<refptr<pool> >::iterator it = pools.begin(); it != pools.end(); it++)
    *it = pool::create_instance();
  //size_t clock_end = clock();
  LARGE_INTEGER clock_end;
  ::QueryPerformanceCounter(&clock_end);

  LARGE_INTEGER clock_freq;
  ::QueryPerformanceFrequency(&clock_freq);

  //size_t num_clocks = clock_end - clock_begin;
  long long num_clocks = (clock_end.QuadPart - clock_begin.QuadPart)*1000/clock_freq.QuadPart;
  SINET_VALIDATE_INT64("poolcreation", num_clocks < 500, num_clocks);

  printf("sleeping 20 seconds to observe CPU utilization ...\n");
  ::Sleep(20000);
}

void run_test_sampledownload()
{
  printf("--------------------------------------------\n");
  printf("run_test_sampledownload\n");
  refptr<request> request = request::create_instance();
  //request->set_request_url(L"http://www.plu.cn/");
  //request->set_request_url(L"https://www.shooter.cn/tmp/alu.jpg");
  //request->set_request_url(L"http://www.shooter.cn/tmp/alu.jpg");
  request->set_request_url(L"http://dl.baofeng.com/storm3/Storm2012-3.10.09.05.exe");
  refptr<task> task = task::create_instance();
  task->append_request(request);
  refptr<pool> pool = pool::create_instance();
  pool->execute(task);
  printf("executing task\n");
  size_t response_size_last = 0;
  clock_t last_clock = clock();
  size_t kbps = 0;
  while (pool->is_running_or_queued(task))
  {
    if (request->get_response_size() != response_size_last &&
      clock() - last_clock > CLOCKS_PER_SEC)
    {
      kbps = (request->get_response_size()-response_size_last)/1024/((clock()-last_clock)/CLOCKS_PER_SEC);
      response_size_last = request->get_response_size();
      last_clock = clock();
      printf("response: %d (%d KB/s)\n", response_size_last, kbps);
    }
    ::Sleep(50);
    if (response_size_last > 3000000)
    {
      printf("canceling operation ... \n");
//       pool->cancel(task);
      break;
    }
  }
  printf("task complete, size: %d\n", request->get_response_size());

  SINET_VALIDATE_INT("sampledownload", response_size_last > 0, response_size_last);
}

int main(int argc, char* argv[])
{
  printf("sleeping 5 secs before test start ...\n\n");
  ::Sleep(5000);

  run_test_poolcreation();
  run_test_sampledownload();

  printf("\nsleeping 5 secs after test end ...\n");
  ::Sleep(5000);
	return 0;
}
