#include <iostream>
#include <unistd.h>

#include "include/threadpool.h"

using namespace std;

void printfuc(int s){
    cout << s << std::endl;
    usleep(1000);
}


int main() {
    Threadpool mpool(8);
    mpool.init_on();
    for(int i = 0; i < 1000;i++) {
        mpool.submit(printfuc,i);
    }
    for(int i = 1000; i < 2000;i++) {
        mpool.submit(printfuc,i);
    }
    mpool.shutdown();
}