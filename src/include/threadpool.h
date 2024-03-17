#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

#include "utils.h"


class Threadpool
{

public:
    class function_class
    {
    public:
        function_class(void (*fuc)(void *,void **),void *argc,void *argv[]) : func(fuc) , argc(argc) ,argv(argv) { }
        void (*func)(void *,void **);
        void *argc;
        void **argv;
    };
    Threadpool(int thread_num = 8);
    ~Threadpool();
    bool submit(void (*fuc)(void *,void **), void *argc, void *argv[]);
    void shutdown();
    void work_thread();
private:
    WEBSER::safe_queue<function_class> works_;
    std::vector<std::thread> threads_;
    int thread_num_;
    std::mutex lock_;
    bool is_active_;
    std::condition_variable condition_;
};





