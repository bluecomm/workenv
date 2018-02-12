#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main() {
    srand(time(NULL));
    int retry_interval = 2;
    for (int i = 0; i < 5; ++i) {
        int sleep_time = retry_interval + rand() % 3;
        sleep(sleep_time);
        std::cout << "sleeping time: " << sleep_time << std::endl;
    }
    return 0;
}
