## sinet-lib manual ##
### pool ###
task运行的容器，可以通过execute方法加入新task，一个pool中的task是依次运行的，可以通过cancel方法取消某一Task，也可通过is\_running、is\_queued、is\_running\_or\_queued查询某一task的当前状态。
### task ###
pool中运行的单位，包含一个或多个request，一个task中的request是并行执行的，可以通过对应方法增删其中的Request。一个Task还对应着一个observer以及一个config，可以通过attach\_observer、detach\_observer、get\_observer、use\_config、get\_config进行设置存取。另外，每个task都有一个当前状态的标记，如taskstatus\_initial、taskstatus\_queued等，该状态属性只能由task所属的pool在运行过程中进行设置。
### config ###
config为per-task属性，保存着task的proxy、agent等属性，通过get\_strvar、set\_strvar、remove\_strvar进行某一属性的获取、设置和删除。
### request ###
一个request表示一个http请求，是一个task中的一个元素，多个request在一个task中并行处理。在一个request中含有多组set/get方法，用以对request\_method、request\_url、request\_header、postdata等属性进行存取。
### postdataelem ###
postdataelem为一个postdata的组成单位，包含一个POST请求中的一个文件或buffer或text等，并有name、type等属性可以通过相应方法进行存取。
### postdata ###
一个POST请求对应着一个postdata，一个postdata中含有一个或多个postdataelem，可以通过add\_elem、remove\_elem、get\_elements、clear、get\_element\_count等方法进行增删、获取和数量查询。

## SiNet中智能指针的应用 ##
SiNet中的对象都使用智能指针进行封装，如：
> ` refptr<request> `
等等。智能指针对象中含有一个引用计数器， AddRef和Release对引用计数器进行加减，当引用计数器的值为0时对该指针进行内存释放。此处还通过运算符重载将赋值运算符映射为调用AddRef方法（右值不为NULL）或Release方法（右值为NULL）。

## SiNet的多层封装说明 ##
结构简图如下：
![http://sinet-lib.googlecode.com/files/sinet.png](http://sinet-lib.googlecode.com/files/sinet.png)
由于dll-boundry问题的存在，同一内存的分配和销毁必须限定在App.exe与sinet\_dyn\_wrapper或sinet\_dyn与sinet中。

## 使用举例 ##
简单HTTP-GET：
```
  // 声明需要的各个变量
  sinet::refptr<sinet::pool>    net_pool = sinet::pool::create_instance();
  sinet::refptr<sinet::task>    net_task = sinet::task::create_instance();
  sinet::refptr<sinet::request> net_rqst = sinet::request::create_instance();
  sinet::refptr<sinet::config>  net_cfg  = sinet::config::create_instance();
  // 配置config
  net_task->use_config(net_cfg);
  // 设置request的各个属性
  net_rqst->set_request_url(L”http://host/rqsturl.php?action=tryget”);
  net_rqst->set_request_method(REQ_GET);
  // 将request添加到对应的task中
  net_task->append_request(net_rqst);
  // 将task添加到对应的pool中
  net_pool->execute(net_task);
  // 等待task执行结束
  while (net_pool->is_running_or_queued(net_task))
  {
    if (::WaitForSingleObject(m_stopevent, 1000) == WAIT_OBJECT_0)
      return;
  }
  // 获取response buffer
  std::vector<unsigned char> st_buffer = net_rqst->get_response_buffer();
```
简单HTTP-POST：
```
  sinet::refptr<sinet::pool>     net_pool = sinet::pool::create_instance();
  sinet::refptr<sinet::task>     net_task = sinet::task::create_instance();
  sinet::refptr<sinet::request>  net_rqst = sinet::request::create_instance();
  sinet::refptr<sinet::postdata> net_pd   = sinet::postdata::create_instance();

  //构造两个postdataelem并添加到postdata中
  sinet::refptr<sinet::postdataelem> net_pelem1 = sinet::postdataelem::create_instance();
  net_pelem1->set_name(L"file");
  net_pelem1->setto_file(“myfile.txt”);
  net_pd->add_elem(net_pelem1);

  sinet::refptr<sinet::postdataelem> net_pelem2 = sinet::postdataelem::create_instance();
  net_pelem3->set_name(L"action");
  net_pelem3->setto_text(L"trypost");
  net_pd->add_elem(net_pelem2);

  net_rqst->set_request_url(L”http://host/rqsturl.php”);
  net_rqst->set_request_method(REQ_POST);
  net_rqst->set_postdata(net_pd);
  net_task->append_request(net_rqst);
  net_pool->execute(net_task);

  while (net_pool->is_running_or_queued(net_task))
  {
    if (::WaitForSingleObject(m_stopevent, 1000) == WAIT_OBJECT_0)
      return;
  }
```
一个pool执行多个task时的处理：
```
  sinet::refptr<sinet::pool> net_pool = sinet::pool::create_instance();  
  std::vector<sinet::refptr<sinet::task>> net_tasks;
  // fill the vector
  if (net_tasks.empty())
    return;
  sinet::refptr<sinet::task> lasttask;
  // 由于task的添加和执行是异步的，所以可以将全部task添加到pool中并无需等待
  for (std::vector<sinet::refptr<sinet::task>>::iterator it = net_tasks.begin();
       it != net_tasks.end(); it++)
  {
    net_pool->execute(*it);
    // lasttask记录最后一个添加的task
    lasttask = *it;
  }
  // 等待最后一个task执行结束，由于pool中所有task顺序执行，所以最后一个task执行结束则表明所有task执行结束
  while (net_pool->is_running_or_queued(lasttask))
  {
    if (::WaitForSingleObject(m_stopevent, 1000) == WAIT_OBJECT_0)
      return;
  }
```
取消一个任务：
```
  if (pool->is_running_or_queued(task))\
  {
    // 如果需要 cancel
    if (if_current_task_should_be_killed)
       pool->cancel(task);
  }
```