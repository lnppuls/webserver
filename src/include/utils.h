#pragma once
#include <mutex>
#include <condition_variable>
#include <queue>

namespace WEBSER {

template <typename T>
class safe_queue
{
public:
    safe_queue() { }
    ~safe_queue() { }
    void push(T &t) {
        std::unique_lock<std::mutex> lock_gruad(lock_);
        data_.emplace(t);
    }
    bool pop(T &t) {
        std::unique_lock<std::mutex> lock_gruad(lock_);
        if(data_.empty())
            return false;
        t = std::move(data_.front());
        data_.pop();
        return true;
    }
    int size() {
        std::unique_lock<std::mutex> lock_gruad(lock_);
        return data_.size();
    }
    bool empty(){
        std::unique_lock<std::mutex> lock_gruad(lock_);
        return data_.size() == 0;
    }
private:
    std::queue<T> data_;
    std::mutex lock_;
};


}
