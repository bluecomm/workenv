/**
 * 20160909
 */
#include <iostream>
#include <vector>
#include "base64.h"

using namespace std;

typedef int32_t (*EncodeDecodeFuc)(const std::string &, std::string &);

int main(){
    EncodeDecodeFuc encode_func = encode;
    EncodeDecodeFuc decode_func = decode;
    //EncodeDecodeFuc encode_func = base64_encode;
    //EncodeDecodeFuc decode_func = base64_decode;

    vector<string> v_input_str;

    //v_input_str.push_back("majing majing 4488 2016-09-09 14:30 base64.cpp");
    //v_input_str.push_back("[+] Relay: Trying 'DoorGod' authorization,this will take a while...");
    v_input_str.push_back("[-] Pls register it on noah.baidu.com. Reference: http://noah.baidu.com/help/faq_noah.html#n2");
    v_input_str.push_back("He said: \"Hello world !\"");
    v_input_str.push_back("ÄãºÃ°¡");
    v_input_str.push_back("ÑÏ");
    v_input_str.push_back("a");
    v_input_str.push_back("ab");
    v_input_str.push_back("abc");
    v_input_str.push_back("abcd");
    v_input_str.push_back("abcde");
    v_input_str.push_back("abcdef");
    v_input_str.push_back("abcdefg");
    v_input_str.push_back("abcdefgh");
    v_input_str.push_back("abcdefghi");


    string encode_output_str;
    string decode_output_str;

    for (vector<string>::iterator it = v_input_str.begin(); it != v_input_str.end(); it ++){

        encode_output_str.clear();
        decode_output_str.clear();

        string input_str = *it;
        
        cout << "Input  : " << input_str << endl;
        encode_func(input_str, encode_output_str);
        cout << "Encoded: " << encode_output_str << endl; 
    
        decode_func(encode_output_str, decode_output_str);
        cout << "Decoded: " << decode_output_str << endl;
    
        if (input_str == decode_output_str){
            cout << "encode and decode ok !" << endl;
        } else {
            cout << " encode and decode failed !" << endl;
        }
    }

    return 0;
}
