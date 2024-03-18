#pragma once

#include <cstring>
#include <string>
#include <assert.h>
#include <atomic>
#include <iostream>

#include "http.h"
#include "threadpool.h"
#include "utils.h"

#define MAX_EVENT_NUMBER 10000

class Webserver
{
public:
    Webserver();
    ~Webserver();

    void init(int threadnum,int port);
    void init_server();
    
    void listen_loop();

    int deal_connect(int fd);
    int deal_write(int fd);
    int deal_read(int fd);



private:

    bool is_up;
    
    // socket相关
    int mport_;
    int mlistened_fd;

    // 线程池相关
    std::shared_ptr<Threadpool> mthreadpool_;
    int mthread_num_;

    //epoll相关
    epoll_event events_[MAX_EVENT_NUMBER];
    int mepollfd_;

    std::shared_ptr<UTILS::Utils> mutils_;

};