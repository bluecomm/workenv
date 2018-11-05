#include <map>
#include <string>
#include <iostream>

int main {
    std::map<std::string, std::string> dict_name = {
        {"a", "1"},
        {"b", "2"},
        {"c", "3"}
    };
    for (std::map<std::string, std::string>::iterator ite = dict_name.begin();
            ite != dict_name.end(); ++ite) {
        std::cout << ite->first << " -> " << ite->second << std::endl;
    }
    return 0;
}
