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
    void push(T t) {
        std::unique_lock<std::mutex> lock_gruad(lock_);
        data_.push(t);
    }
    T pop() {
        std::unique_lock<std::mutex> lock_gruad(lock_);
        T res = data_.back();
        data_.pop();
        return res;
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
