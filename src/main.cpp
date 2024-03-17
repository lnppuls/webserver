#include <iostream>
#include <unistd.h>

#include "include/threadpool.h"

using namespace std;

void printfuc(int s){
    cout << s << std::endl;
}


int main() {
    Threadpool mpool(8);
    mpool.init_on();
    for(int i = 0; i < 1000;i++) {
        mpool.submit(printfuc,i);
    }
    sleep(3);
    for(int i = 0; i < 10000;i++) {
        mpool.submit(printfuc,i);
    }
    mpool.shutdown();
}