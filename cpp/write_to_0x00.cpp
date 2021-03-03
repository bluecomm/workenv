#include<stdio.h>
#include<string.h>

int main () {

    int* p = 0x00;
    printf("p addr: %p\n", p);
    printf("p str: %s\n", p);
    p[0] = 'a';
    printf("p str: %s\n", p);

    char p_str[32] = {'a', 'b', 'c', '1', '2', '3'};
    printf("p str: %s\n", p_str);

    memcpy(p_str, p, sizeof(p_str));
    printf("p str: %s\n", p);

    return 0;
}
