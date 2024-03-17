#include "./include/threadpool.h"
#include "threadpool.h"


Threadpool::Threadpool(int thread_num) : thread_num_(thread_num)
{
    std::unique_lock<std::mutex> lk(lock_);
    is_active_ = true;
    for(int i = 0; i < thread_num; i++) {
        threads_.emplace_back(std::thread (&Threadpool::work_thread,this));
    }
}

Threadpool::~Threadpool()
{
    this->shutdown();
}
bool Threadpool::submit(void (*fuc)(void *,void **), void *argc, void *argv[])
{
    if(!is_active_) 
        return false;
    works_.push(function_class {fuc,argc,argv});
    condition_.notify_one();
    return true;
}

void Threadpool::shutdown()
{
    lock_.lock();
    is_active_ = false;
    lock_.unlock();
    for(int i = 0; i < thread_num_; i++) {
        threads_[i].join();
    }
}

void Threadpool::work_thread()
{
    while (!works_.empty() || is_active_)
    {   
        function_class mfucs{nullptr,nullptr,nullptr};
        {
            std::unique_lock<std::mutex> lc(lock_);
            if(works_.empty()) {
                condition_.wait(lc);
            }
            mfucs = works_.pop();
        }
        mfucs.func(mfucs.argc,mfucs.argv);
    }
}
