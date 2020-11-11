#include <stdio.h>
//#include <stdlib.h>
#include <unistd.h>
//#include <time.h>
 
void fun1() {
    printf("fun1\n");
    sleep(1);
    fun1();
}

int main() {
    fun1();
    return 0;
}
