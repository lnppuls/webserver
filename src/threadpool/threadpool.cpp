#include "../include/threadpool.h"


Threadpool::~Threadpool()
{
    if (is_active_) 
        this->shutdown();
}

void Threadpool::init_on()
{
    is_active_.store(true);
    for(int i = 0; i < thread_num_; i++) {
        threads_.emplace_back(std::thread(&Threadpool::worker,this));
    }
}

void Threadpool::shutdown()
{
    is_active_.store(false);
    condition_.notify_all();
    for(int i = 0; i < thread_num_; i++) {
        threads_[i].join();
    }
    threads_.clear();
}

void Threadpool::worker()
{
    while (is_active_.load() || !works_.empty())
    {
        std::function<void ()> task;
        bool ret = false;
        {
            std::unique_lock<std::mutex> lk(lock_);
            if(!works_.empty()) {
                ret = works_.pop(task);
            } else {
                if(!is_active_)
                    break;
                condition_.wait(lk);
            }
        }
        if(ret)
            task();
    }
}