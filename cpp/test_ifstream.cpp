#include <fstream>
#include <iostream>

int main() {

    std::string raw_data;
    std::ifstream ifs("test_data.txt");
    while (!ifs.eof()) {
        std::string tmp_data;
        //ifs >> tmp_data;
        getline(ifs, tmp_data);
        raw_data += tmp_data + "\n";
    }
    std::cout << raw_data << std::endl;
    ifs.close();
    /*
    std::string line;
    std::ifstream ifs("test.txt");
    while (!ifs.eof()) {
        std::cout << ifs.eof() << " " << getline(ifs, line) << " " <<  ifs.eof() << " " << line << std::endl;
    }
    ifs.close();

    std::ifstream ifs1("test.txt");
    int ret = 0;
    while (getline(ifs1, line)) {
        std::cout << ifs1.eof() << " " << line << std::endl;
    }
    ifs1.close();
    */

    return 0;
}
