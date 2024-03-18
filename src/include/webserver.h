#pragma once

#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

#include "http.h"
#include "threadpool.h"
#include "utils.h"

class Webserver
{
public:
    Webserver();
    ~Webserver();

private:
    // socket相关
    int mport_;
    
    int mepollfd_;

    // 线程池相关
    std::shared_ptr<Threadpool> mthreadpool_;
    int mthread_num_;

    //
    
};