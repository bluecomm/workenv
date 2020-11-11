#include <string>
#include <iostream>
#include <sstream>
#include <stdint.h>

uint64_t string2integer(const std::string& in_str) {                                          
    std::stringstream sstream("");
    uint64_t num;                                                        
    sstream << in_str;                                                                                   
    sstream >> num;                                                                                      
    return num;                                                                                         
} 

std::string integer2string(uint64_t in_int) {
    std::stringstream sstream("");
    uint64_t num;                                                        
    std::string out_str;
    sstream << in_int;                                                                                   
    sstream >> out_str;                                                                                      
    return out_str;
    //return sstream.str();                                                                                         
} 

int main() {
    /*
    std::string num_str;
    while(true) {
        std::cout << "Input: ";
        std::cin >> num_str;
        std::cout << string2integer(num_str) << std::endl;
    }
    */
    uint64_t in_int;
    while(true) {
        std::cout << "Input: ";
        std::cin >> in_int;
        std::cout << integer2string(in_int) << std::endl;
    }
    return 0;
}
