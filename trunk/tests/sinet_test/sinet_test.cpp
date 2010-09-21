#include "pch.h"
#include "../../sinet/sinet.h"
#include <time.h>
#include <iostream>

using namespace sinet;

int main()
{
  refptr<config> cfg = config::create_instance();
  std::wstring strproxy;
  if (!cfg->get_strvar(CFG_STR_PROXY, strproxy))
    std::wcout << L"CANNOT FIND CFG_STR_PROXY\n" << std::endl;

  cfg->set_strvar(CFG_STR_PROXY, L"I'M PROXY!!!");
  if (cfg->get_strvar(CFG_STR_PROXY, strproxy))
    std::wcout << L"FOUND PROXY~ IT'S:" << std::endl << strproxy.c_str() << "\n" << std::endl;

  cfg->remove_strvar(CFG_STR_PROXY);
  if (!cfg->get_strvar(CFG_STR_PROXY, strproxy))
    std::wcout << L"CANNOT FIND CFG_STR_PROXY AGINE...\n" << std::endl;
  return 0;
}

//#define SINET_VALIDATE_INT(test_name, condition, variable) \
//  { \
//    if (##condition)\
//      printf ("test [%s] is [OK]\n", test_name);\
//    else\
//      printf ("test [%s] [FAILED], condition requires (" #condition ") and its value is: %d\n", test_name, variable);\
//    \
//  }
//
//#define SINET_VALIDATE_INT64(test_name, condition, variable) \
//{ \
//  if (##condition)\
//  printf ("test [%s] is [OK]\n", test_name);\
//    else\
//    printf ("test [%s] [FAILED], condition requires (" #condition ") and its value is: %lld\n", test_name, variable);\
//    \
//}
//
//#if defined(_WINDOWS_)
//#define CLOCKS_PER_SECOND CLOCKS_PER_SEC
//#elif defined(_MAC_)
//#define CLOCKS_PER_SECOND 10000
//#endif
//
//void run_test_poolcreation()
//{
//  printf("--------------------------------------------\n");
//  printf("run_test_poolcreation\n");
//  int num_pools = 1000;
//  printf("start timing for %d pool creation ...\n", num_pools);
//  size_t clock_begin = clock();
//  std::vector<refptr<pool> > pools(num_pools);
//  for (std::vector<refptr<pool> >::iterator it = pools.begin(); it != pools.end(); it++)
//    *it = pool::create_instance();
//  size_t clock_end = clock();
//
//  size_t num_clocks = clock_end - clock_begin;
//  //SINET_VALIDATE_INT64("poolcreation", num_clocks < 500, num_clocks);
//
//  printf("sleeping 20 seconds to observe CPU utilization ...\n");
//#if defined (_WINDOWS_)
//  ::Sleep(20000);
//#elif defined (_MAC_)
//  sleep(2);
//#endif
//}
//
//void run_test_sampledownload()
//{
//  printf("--------------------------------------------\n");
//  printf("run_test_sampledownload\n");
//  refptr<request> request = request::create_instance();
//  //request->set_request_url(L"http://www.plu.cn/");
//  //request->set_request_url(L"https://www.shooter.cn/tmp/alu.jpg");
//  //request->set_request_url(L"http://www.shooter.cn/tmp/alu.jpg");
//  request->set_request_url(L"http://dl.baofeng.com/storm3/Storm2012-3.10.09.05.exe");
//  refptr<task> task = task::create_instance();
//  task->append_request(request);
//  refptr<pool> pool = pool::create_instance();
//  pool->execute(task);
//  printf("executing task\n");
//  size_t response_size_last = 0;
//  clock_t last_clock = clock();
//  size_t kbps = 0;
//  while (pool->is_running_or_queued(task))
//  {
//    // CLOCKS_PER_SEC is defined as 1000000 on Mac OS X 64 bit
//    // but when compiling a 32 bit program, it looks this value 
//    // should be something around 10000
//    // Windows don't have this problem, the definition is 1000
//    if (request->get_response_size() != response_size_last &&
//      clock() - last_clock > CLOCKS_PER_SECOND)
//    {
//      kbps = (request->get_response_size()-response_size_last)/1024/((clock()-last_clock)/CLOCKS_PER_SECOND);
//      response_size_last = request->get_response_size();
//      last_clock = clock();
//      printf("response: %d (%d KB/s)\n", response_size_last, kbps);
//    }
//#if defined(_WINDOWS_)
//    ::Sleep(50);
//#elif defined(_MAC_)
//    usleep(50000);
//#endif
//    if (response_size_last > 3000000)
//    {
//      printf("canceling operation ... \n");
//      pool->cancel(task);
//      break;
//    }
//  }
//  printf("task complete, size: %d\n", request->get_response_size());
//
//  //SINET_VALIDATE_INT("sampledownload", response_size_last > 0, response_size_last);
//}
//
//int main(int argc, char* argv[])
//{
//  printf("sleeping 5 secs before test start ...\n\n");
//#if defined(_WINDOWS_)
//  ::Sleep(5000);
//#elif defined(_MAC_)
//  sleep(5);
//#endif
//
//  run_test_poolcreation();
//  run_test_sampledownload();
//
//  printf("\nsleeping 5 secs after test end ...\n");
//#if defined(_WINDOWS_)
//  ::Sleep(5000);
//#elif defined(_MAC_)
//  sleep(5);
//#endif
//	return 0;
//}
