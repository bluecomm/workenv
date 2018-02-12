#include <string>
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>     
#include <boost/tokenizer.hpp>     
#include <boost/algorithm/string.hpp>
#include <iostream>

int main() {
    std::string aa = "123";
    //int a = static_cast<int>(aa);
    //int a = dynamic_cast<int>(aa);
    int aaa = boost::lexical_cast<int>(aa);
    std::cout << aaa << std::endl;

    std::string bb = "123.4";
    float bbb = boost::lexical_cast<float>(bb);
    std::cout << bbb << std::endl;

    std::string str_test;
    std::vector<std::string> v_1;
    boost::split(v_1, str_test, boost::is_any_of(","));
    for (size_t i = 0; i < v_1.size(); ++i) {
        std::cout << v_1[i] << std::endl;
    }

    return 0;
}
