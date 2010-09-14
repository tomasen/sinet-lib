#include "pch.h"
#include "../../sinet/sinet.h"
#include <time.h>

using namespace sinet;

int main(int argc, char* argv[])
{
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
    if (response_size_last > 5000000)
    {
      printf("canceling operation ... \n");
//       pool->cancel(task);
      break;
    }
  }
  printf("task complete, size: %d\n", request->get_response_size());
	return 0;
}
