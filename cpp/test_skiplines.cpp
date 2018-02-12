#include <fstream>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " filename " << "skipped_line_num" << std::endl;
        return 0;
    }
    int skipped_line_num = atoi(argv[2]);
    std::cout << "filename: " << argv[1] << " skipped_line_num: " << skipped_line_num <<std::endl;

    std::string line;
    std::ifstream ifs(argv[1]);
    int line_num_read = 0;
    while ((!ifs.eof()) && line_num_read < skipped_line_num) {
        getline(ifs, line);
        ++ line_num_read;
    }

    while (!ifs.eof()) {
        if (getline(ifs, line)) {
            std::cout << line << std::endl;
        } else {
            std::cout << "end" << std::endl;
        }
    }
    ifs.close();

    return 0;
}
