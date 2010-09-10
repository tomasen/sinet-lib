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
  while (pool->is_running(task))
  {
    if (request->get_response_size() != response_size_last &&
      clock() - last_clock > CLOCKS_PER_SEC)
    {
      response_size_last = request->get_response_size();
      last_clock = clock();
      printf("response: %d\n", response_size_last);
    }
  }
  printf("task complete, size: %d\n", request->get_response_size());
	return 0;
}
