//main.cpp
#include <iostream>
#include "sha512.h"
using namespace std;
  
int main(int argc, char *argv[])
{
    if ( argc!= 2) {
        cout << "please input text" << endl;
        return -1;
    }
    string plain_text = argv[1];
    string sha512_code= sha512(plain_text);
    cout << sha512_code << "\t" << plain_text << endl;
    return 0;
}
