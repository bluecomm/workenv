#include<stdio.h>

int printf(const char *format, ... );  
  
int global_init_var = 16;  
int global_unint_var;  
const char* test_str = "Hello World";
  
void func1 (int );  
  
int main() {  
    static int static_var = -32;  
    static int static_var_uninit;  
  
    int a = 1;  
    int b;  
  
    func1(static_var + global_init_var + a + b);  

    printf("%s\n", test_str);
  
    return a;  
}  
  
void func1 (int i) {  
    printf("%d\n", i);  
}  
