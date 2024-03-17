#include <iostream>
#include <unistd.h>

#include "include/threadpool.h"

using namespace std;

void printfuc(void *argc,void **argv){
    cout << "in" << std::endl;
}

void printfuc2(void *argc,void **argv){
    cout << "out" << std::endl;
}

int main() {
    Threadpool mthreadpool(4);
    for(int i = 0; i < 10000; i++) {
        mthreadpool.submit(printfuc,nullptr,nullptr);
    }
    mthreadpool.shutdown();
}