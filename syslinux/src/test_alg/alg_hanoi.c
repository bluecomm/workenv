#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int counter = 0;

void move(int n, char x, char z){
    counter++;
    printf("[%d] %d: %c --> %c\n", counter, n, x, z);
    return;
}


void hanoi(int n, char x, char y, char z){
    if (n == 1){
        move(1, x, z);
    } else {
        hanoi(n-1, x, z, y);
        move(n, x, z);
        hanoi(n-1, y, x, z);
    }
    return;
}


int main(int argc, char **argv){
    int num = 3;
    if (argc == 2)
        num = atoi(argv[1]);
    hanoi(num, 'x', 'y', 'z');
    return 0;
}

