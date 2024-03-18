#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <atomic>

#include "utils.h"


class Threadpool
{

public:
    
    Threadpool(int thread_num = 8) : thread_num_(thread_num) { }
    ~Threadpool();

    void init_on();
    void shutdown();

    template<typename F,typename...Args>
    auto submit(F && f,Args && ...args) -> std::future<decltype(f(args...))>
    {        
        std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f),std::forward<Args>(args)...);
        auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>> (func);
        std::function<void ()> wrapper_func = [task_ptr]()
        {
            (*task_ptr)();
        };
        works_.push(wrapper_func);
        condition_.notify_one();
        
        return task_ptr->get_future();
    }

    void worker();

private:
    UTILS::safe_queue<std::function<void()>> works_;
    std::vector<std::thread> threads_;
    int thread_num_;
    std::mutex lock_;
    std::atomic<bool> is_active_;
    std::condition_variable condition_;
};
