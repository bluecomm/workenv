#include <string>
#include <iostream>

int convert_str(const std::string& src, std::string& dest) {
    for (size_t i = 0; i < src.size(); ++i) {
        if ((src[i] & 0x80) == 0) {
            // for english char
            if (src[i] == '\'' || src[i] == '\\') {
                dest += '\\';
            }   
        }   
        dest += src[i];
    } 
    return 0;
}

int main() {

    std::string asaa = "3:1,2,3";
    size_t pos = asaa.find(':');
    std::cout << pos <<  "/" << std::string::npos << std::endl;
    std::cout << asaa.substr(0,pos) << "/" <<  asaa.substr(pos+1) << std::endl;
    pos = asaa.find('a');
    std::cout << pos <<  "/" << std::string::npos << std::endl;

    char src_char[] = {'h', 'e', 'l', '\\', 'l', 'o', '\''};
    std::string src1 = src_char;
    std::cout << "src1: " << src1 << std::endl;
    std::cout << src1.compare("hel\\lo\'") << std::endl;
    std::string dest1;
    convert_str(src1, dest1);
    std::cout << "after convert: " << dest1 << std::endl;
    std::cout << dest1.compare("hel\\\\lo\\\'") << std::endl;
    
    std::string dest2 = dest1.c_str();
    std::cout << "after=: " << dest2 << std::endl;

    return 0;
}
