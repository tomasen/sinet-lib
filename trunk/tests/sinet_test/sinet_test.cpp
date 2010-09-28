#include "pch.h"
#include "../../sinet/sinet.h"
#include "../../sinet/pool_impl.h"
#include <time.h>
#include <fstream>
#include <Shlwapi.h>

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

#if defined(_WINDOWS_)
#define CLOCKS_PER_SECOND CLOCKS_PER_SEC
#elif defined(_MAC_)
#define CLOCKS_PER_SECOND 10000
#endif

void run_test_poolcreation()
{
  printf("--------------------------------------------\n");
  printf("run_test_poolcreation\n");
  int num_pools = 1000;
  printf("start timing for %d pool creation ...\n", num_pools);
  size_t clock_begin = clock();
  std::vector<refptr<pool> > pools(num_pools);
  for (std::vector<refptr<pool> >::iterator it = pools.begin(); it != pools.end(); it++)
    *it = pool::create_instance();
  size_t clock_end = clock();

  size_t num_clocks = clock_end - clock_begin;
  //SINET_VALIDATE_INT64("poolcreation", num_clocks < 500, num_clocks);

  printf("sleeping 20 seconds to observe CPU utilization ...\n");
#if defined (_WINDOWS_)
  ::Sleep(20000);
#elif defined (_MAC_)
  sleep(2);
#endif
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
    // CLOCKS_PER_SEC is defined as 1000000 on Mac OS X 64 bit
    // but when compiling a 32 bit program, it looks this value 
    // should be something around 10000
    // Windows don't have this problem, the definition is 1000
    if (request->get_response_size() != response_size_last &&
      clock() - last_clock > CLOCKS_PER_SECOND)
    {
      kbps = (request->get_response_size()-response_size_last)/1024/((clock()-last_clock)/CLOCKS_PER_SECOND);
      response_size_last = request->get_response_size();
      last_clock = clock();
      printf("response: %d (%d KB/s)\n", response_size_last, kbps);
    }
#if defined(_WINDOWS_)
    ::Sleep(50);
#elif defined(_MAC_)
    usleep(50000);
#endif
    if (response_size_last > 3000000)
    {
      printf("canceling operation ... \n");
      pool->cancel(task);
      break;
    }
  }
  printf("task complete, size: %d\n", request->get_response_size());

  //SINET_VALIDATE_INT("sampledownload", response_size_last > 0, response_size_last);
}

  /****
    测试方法：
      1、将两个待上传的文件放入目录
      2、设置UPLOADFILE、UPLOADBUFFER，分别为待上传文件名；
         设置UPLOADBUFFER_FILENAME，它是指定其中被用于buffer上传时指定的文件名
      3、在服务器端test_sinet.php同级目录下，放置用于验证两个上传文件的原文件
         在test_sinet.php里设置$file1和$file2，分别为原文件所在路径
  */
#define REQ_ACT_GET     L"?act=get"
#define REQ_ACT_COOKIE  L"?act=cookie"
#define REQ_ACT_FORM    L"?act=form"
#define REQ_ACT_FILE    L"?act=file"

#define UPLOADFILE    L"form_putty.jpg"
#define UPLOADBUFFER  L"form_buffer.jpg"
#define UPLOADBUFFER_FILENAME   L"form_buffer.jpg"

refptr<request> create_test_req(std::wstring url, std::wstring act, std::wstring filepath)
{
  if (act.empty())
    return NULL;

  refptr<request> request = request::create_instance();

  std::wstring requrl = url + act;
  request->set_request_url(requrl.c_str());

  if (act == REQ_ACT_GET)
  {
    request->set_request_method(REQ_GET);
  }
  else if (act == REQ_ACT_COOKIE)
  {
    si_stringmap header;
    header[L"Cookie"] = L"a1=1234567; a2=cookie; a3=%E8%BF%99%E6%98%AFcookie; a4=%E6%B5%8B%E8%AF%95%E6%B5%8B%E8%AF%95";
    request->set_request_header(header);
  }
  else if (act == REQ_ACT_FORM)
  {
    request->set_request_method(REQ_POST);
    refptr<postdata> postdata = postdata::create_instance();

    refptr<postdataelem> elem1 = postdataelem::create_instance();
    elem1->set_name(L"a1");
    elem1->setto_text(L"测试表单");
    postdata->add_elem(elem1);

    refptr<postdataelem> elem2 = postdataelem::create_instance();
    elem2->set_name(L"a2");
    elem2->setto_text(L"testform");
    postdata->add_elem(elem2);

    refptr<postdataelem> elem3 = postdataelem::create_instance();
    elem3->set_name(L"a3");
    elem3->setto_text(L"12345678");
    postdata->add_elem(elem3);

    refptr<postdataelem> elem4 = postdataelem::create_instance();
    elem4->set_name(L"a4");
    elem4->setto_text(L"测试form");
    postdata->add_elem(elem4);

    refptr<postdataelem> elem5 = postdataelem::create_instance();
    elem5->set_name(L"a5");
    elem5->setto_text(L"123123form");
    postdata->add_elem(elem5);

    // 上传文件
    refptr<postdataelem> elem6 = postdataelem::create_instance();
    elem6->set_name(L"form_upload1");
    std::wstring file = filepath + L"\\" + UPLOADFILE;
    elem6->setto_file(file.c_str());
    postdata->add_elem(elem6);

    // 通过buffer上传文件
    std::ifstream fs;
    file = filepath + L"\\" + UPLOADBUFFER;
    fs.open(file.c_str(), std::ios::binary|std::ios::in);
    if (!fs)
    {
       printf("dose not open form_buffer.jpg!\n");
       return NULL;
    }
    fs.seekg(0, std::ios::end);
    size_t filesize = fs.tellg();
    void* buffer = malloc(filesize);
    fs.seekg(0, std::ios::beg);
    fs.read((char*)buffer, filesize);

    fs.close();
    //printf("filesize = %d\n", filesize);
    refptr<postdataelem> elem7 = postdataelem::create_instance();
    elem7->set_name(L"form_upload2");
    elem7->setto_text(UPLOADBUFFER_FILENAME);
    elem7->setto_buffer(buffer, filesize);
    postdata->add_elem(elem7);

    free(buffer);

    request->set_postdata(postdata);

  }
  else if (act == REQ_ACT_FILE)
  {
  }
  
  return request;
}

int main(int argc, char* argv[])
{
  refptr<pool> pool = pool::create_instance();
  refptr<task> task = task::create_instance();
  refptr<config> cfg = config::create_instance();
  task->use_config(cfg);

  wchar_t path[256];
  GetModuleFileName(NULL, path, 256);
  PathRemoveFileSpecW(path);
  std::wstring filepath = path;

  refptr<request> req_get = create_test_req(L"http://webpj/test_sinet.php", REQ_ACT_FORM, filepath);
  if (!req_get.get())
  {
    printf("req_get error\n");
    return 0;
  }

  req_get->set_request_outmode(REQ_OUTFILE);
  filepath += L"\\req_get.txt";
  req_get->set_outfile(filepath.c_str());

  task->append_request(req_get);
  pool->execute(task);

  size_t response_size_last = 0;
  clock_t last_clock = clock();
  size_t kbps = 0;
  while (pool->is_running_or_queued(task))
  {
//     if (req_get->get_response_size() != response_size_last &&
//       clock() - last_clock > CLOCKS_PER_SECOND)
//     {
//       //kbps = (req_get->get_response_size()-response_size_last)/1024/((clock()-last_clock)/CLOCKS_PER_SECOND);
//       //response_size_last = req_get->get_response_size();
//       last_clock = clock();
//       //printf("response: %d (%d KB/s)\n", response_size_last, kbps);
//     }
  }
  printf("%d,%d, response:%s\n", req_get->get_response_errcode(), req_get->get_response_size(), L"");
  return 0;

  printf("sleeping 5 secs before test start ...\n\n");
#if defined(_WINDOWS_)
  ::Sleep(5000);
#elif defined(_MAC_)
  sleep(5);
#endif

  run_test_poolcreation();
  run_test_sampledownload();

  printf("\nsleeping 5 secs after test end ...\n");
#if defined(_WINDOWS_)
  ::Sleep(5000);
#elif defined(_MAC_)
  sleep(5);
#endif
	return 0;
}
