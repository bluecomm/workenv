#include <stdio.h>
//#include <stdlib.h>
#include <unistd.h>
//#include <time.h>
 
int main();

void func1() {
    printf("func1\n");
    sleep(1);
    main();
}

int main() {
    printf("main\n");
    sleep(1);
    func1();
    return 0;
}
