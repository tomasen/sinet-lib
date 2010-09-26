#include "pch.h"
#include "task_ctocpp.h"

using namespace sinet;

refptr<task> task::create_instance()
{
  _task_t* impl = _task_create_instance();
  if (impl)
    return task_ctocpp::Wrap(impl);
  return NULL;
}

void task_ctocpp::append_request( refptr<request> request_in )
{

}

int task_ctocpp::erase_request( int request_id )
{
  return 0;
}

void task_ctocpp::clearall_requests()
{

}

int task_ctocpp::get_request_count()
{
  return 0;
}

void task_ctocpp::get_request_ids( std::vector<int>& ids_out )
{

}

refptr<request> task_ctocpp::get_request( int request_id )
{
  return NULL;
}

void task_ctocpp::set_status( int status )
{

}

int task_ctocpp::get_status()
{
  return 0;
}

void task_ctocpp::attach_observer( itask_observer* observer_in )
{

}

void task_ctocpp::detach_observer()
{

}

void task_ctocpp::use_config( refptr<config> config )
{

}

refptr<config> task_ctocpp::get_config()
{
  return NULL;
}