#include<iostream>
#include<string>

using namespace std;
int main() {
    string str;
    str = "123456";
    cout << str << endl;
 
    //方法一：使用substr()
    str = str.substr(0, str.length() - 1);
    cout << str << endl;
 
    //方法二：使用erase()
    str.erase(str.end() - 1);
    cout << str << endl;
 
    //方法三：使用pop_back()
    //just for c++ 11: -std=c++11
    //str.pop_back();
    //cout << str << endl;
    return 0;
}
