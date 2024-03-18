#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

namespace UTILS {

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



class Utils
{
public:
    void set_noblock(int fd) {
        int flags = fcntl(fd, F_GETFL, 0);
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    }

    void addfd(int epollfd, int fd, bool one_shot, int TRIGMode)
    {
        epoll_event event;
        event.data.fd = fd;

        if (1 == TRIGMode)
            event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;  //边缘触发
        else
            event.events = EPOLLIN | EPOLLRDHUP;    //水平触发

        //如果对描述符socket注册了EPOLLONESHOT事件，
        //那么操作系统最多触发其上注册的一个可读、可写或者异常事件，且只触发一次。
        if (one_shot)
            event.events |= EPOLLONESHOT;
        epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);

        set_noblock(fd);
    }
};





}
