#include "pch.h"
#include "../../sinet/sinet.h"
#include <time.h>

int main(int argc, char* argv[])
{
  sinet::refptr<sinet::request> request = sinet::request::create_instance();
  sinet::refptr<sinet::task> task = sinet::task::create_instance();
  task->append_request(request);
  sinet::refptr<sinet::pool> pool = sinet::pool::create_instance();
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
      pool->cancel(task);
      break;
    }
  }
  printf("task complete, size: %d\n", request->get_response_size());
	return 0;
}
