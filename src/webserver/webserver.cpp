#include "../include/webserver.h"

Webserver::Webserver()
{
    is_up = false;
}

Webserver::~Webserver()
{
    is_up = false;
    close(mlistened_fd);
    close(mepollfd_);
}

void Webserver::init(int threadnum,int port)
{
    mthread_num_ = threadnum;
    mport_ = port;
    mthreadpool_ = std::make_shared<Threadpool>(threadnum);
    mthreadpool_->init_on();
    is_up = true;
}

void Webserver::init_server()
{
    mlistened_fd = socket(PF_INET,SOCK_STREAM,0);
    assert(mlistened_fd >= 0);
    
    
    //允许在该套接字关闭或终止后，立即重新绑定到相同的地址和端口上，
    //而不会因为之前的连接仍在 TIME_WAIT 状态而导致地址被占用的错误。
    struct linger tmp = {1, 1};
    setsockopt(mlistened_fd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));

    int reuse = 1;
    setsockopt(mlistened_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(mport_);
    int ret = bind(mlistened_fd, (struct sockaddr*)&address, sizeof(address));
    assert(ret >= 0);

    ret = listen(mlistened_fd,5);
    assert(ret >= 0);

    mepollfd_ = epoll_create(5);
    assert(mepollfd_ >= 0);

    mutils_->addfd(mepollfd_,mlistened_fd,0,1);
    listen_loop();
}

void Webserver::listen_loop()
{
    while (is_up)
    {
        //等待所监控文件描述符上有事件的产生
        int number = epoll_wait(mepollfd_,events_ , MAX_EVENT_NUMBER, -1);

        if (number < 0 && errno != EINTR)
        {
            break;
        }
        //对所有就绪事件进行处理
        for (int i = 0; i < number; i++)
        {
            int sockfd = events_[i].data.fd;
            std::cout << "get fd  " << sockfd << std::endl;

            if(sockfd == mlistened_fd) {
                mthreadpool_->submit(
                    [this,sockfd]() {
                        this->deal_connect(sockfd);
                    }
                );
            } else if (events_[i].events & EPOLLIN)
            {
                mthreadpool_->submit(
                    [this,sockfd]() {
                        this->deal_read(sockfd);
                    }
                );
            }
            //处理客户连接上send的数据
            else if (events_[i].events & EPOLLOUT)
            {
                mthreadpool_->submit(
                    [this,sockfd]() {
                        this->deal_write(sockfd);
                    }
                );
            }
            
        }
    }
}

int Webserver::deal_connect(int fd)
{
    struct sockaddr_in client_address;
    socklen_t client_addrlength = sizeof(client_address);
    int connfd = accept(mlistened_fd, (struct sockaddr *)&client_address, &client_addrlength);
    char msg[20] {"connect accept\n"};
    write(connfd,msg,sizeof(msg));
    sleep(1);
    close(connfd);
    return 0;
}

int Webserver::deal_write(int fd)
{
    return 0;
}

int Webserver::deal_read(int fd)
{
    return 0;
}
