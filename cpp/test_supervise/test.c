#include <unistd.h>

int main() {
    int i = 0;
    while (i < 5) {
        ++i;
        sleep(2);
    }
    return 0;
}
