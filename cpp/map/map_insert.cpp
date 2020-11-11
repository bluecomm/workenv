#include <map>
#include <string>
#include <iostream>

int main() {

    // insert, existed kv WILL NOT BE covered
    std::map<std::string, std::string> test_map;
    std::pair<std::map<std::string, std::string>::iterator, bool> ret;
#define MAP_INSERT_LOG \
do { \
    if (ret.second == false) { \
        std::cout << "insert fail" << std::endl; \
    } else { \
        std::cout << "insert succ" << std::endl; \
    } \
} while(0)
    ret = test_map.insert(std::make_pair("a","123"));
    MAP_INSERT_LOG;
    ret = test_map.insert(std::make_pair("b","456"));
    MAP_INSERT_LOG;
    ret = test_map.insert(std::make_pair("c","789"));
    MAP_INSERT_LOG;
    ret = test_map.insert(std::make_pair("a","101"));
    MAP_INSERT_LOG;

    std::map<std::string, std::string>::iterator ite;
    for (ite = test_map.begin(); ite != test_map.end(); ++ite) {
        std::cout << ite->first << " " << ite->second << std::endl;
    }

    // directly set, existed kv WILL BE covered
    std::map<std::string, std::string> test_map1;
    test_map1["a"] = "123";
    test_map1["b"] = "456";
    test_map1["c"] = "789";
    test_map1["a"] = "101";
    for (ite = test_map1.begin(); ite != test_map1.end(); ++ite) {
        std::cout << ite->first << " " << ite->second << std::endl;
    }

    return 0;
}
