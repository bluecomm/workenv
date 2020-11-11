/**
 * 20160909
 */
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "base64.h"
#include "pack.pb.h"

int main() {
    std::string file3 = "antusheng_3.txt";
    std::ifstream if_json_3(file3.c_str());
    std::string str_json_3((std::istreambuf_iterator<char>(if_json_3)),
            std::istreambuf_iterator<char>());
    std::cout << "json string: " << std::endl << str_json_3 << std::endl;

    std::string file1 = "antusheng_1.txt";
    std::ifstream if_json_1(file1.c_str());
    std::string str_json_1((std::istreambuf_iterator<char>(if_json_1)),
            std::istreambuf_iterator<char>());
    std::cout << "json string: " << std::endl << str_json_1 << std::endl;

    NewStyleRepeated nssp;
    nssp.add_new_styles()->set_json(str_json_3);
    nssp.add_new_styles()->set_json(str_json_1);

     std::string nss_repeated_str;
     if (!nssp.SerializeToString(&nss_repeated_str)) {
         return -1;
     }
     std::string new_styles_buf_encoded;
     if (base64_encode(nss_repeated_str, new_styles_buf_encoded) != 0) {
         return -1;
     }

    std::cout << "new_styles_buf_encoded : " << std::endl << new_styles_buf_encoded << std::endl;
    return 0;
}
