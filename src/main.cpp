#include <iostream>
#include <unistd.h>

#include "include/webserver.h"


using namespace std;

void printfuc(int s){
    cout << s << std::endl;
    usleep(1000);
}


int main() {
    Webserver mweb;
    mweb.init(8,1045);
    mweb.init_server();
}