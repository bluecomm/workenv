#include <iostream>
#include <fstream>

int output(std::ostream &out) {
	out << "output hello" << std::endl;
    uint64_t u64 = 1234567890;
    out << u64 << std::endl;
}

int main () {
	std::ofstream ofs("out.txt");
	ofs << "hello" << std::endl;

	output(std::cout);
	output(ofs);

	ofs.close();

	return 0;
}
