#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
//#include <stdio.h>
#include <iostream>

bool g_flag = true;

void signal_hander(int singno) {
    std::cout << "Oh, cached!!!" << std::endl;
    g_flag = false;
}

int main() {
    signal(SIGINT, signal_hander);
    signal(SIGTERM, signal_hander);
    while(g_flag) {
        std::cout << "running..." << std::endl;
        sleep(1);
    }
    return 0;
}
