#include<stdlib.h>
#include<stdio.h>

void exit_fun(){
    printf("exit_fun\n");
}


int main(){
    printf("main\n");
    atexit(exit_fun);
    return 0;
}
