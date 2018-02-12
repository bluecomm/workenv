#include <iostream>

/*
 * (a+b) mod (n) = (a mod n) + (b mod n) mod (n)
 *      1 %m = 1
 *      12 % m  = ( 1 * 10 + 2 ) % m = ( 1 % m * 10 + 2 ) % m
 *      123 % m = ( 12 * 10 + 3) % m = ( 12 % m * 10 + 3) % m
 *      12345678901234567890 % m =(( 1 % m * 10 + 2 £© % m * 10 + 3 ) % m...
 * */

int div_mod(char* ch, int num) {
    int s = 0;
    for(int i=0; ch[i]!='\0'; ++i) {
        s = (s * 10 + ch[i] - '0') % num;
    }
    return s;
}

int main(int argc, char** argv) {
    int m = 4097;
    if (argc == 2) {
        std::cout << div_mod(argv[1], 4097) << std::endl;
    } else {
        std::cout << "bad input" << std::endl;
    }
    return 0;
}
