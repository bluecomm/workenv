#include <iostream>
#include <vector>
#include <stdint.h>

template <typename T>
void print_vector(std::vector<T> &vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}

int main () {
    std::vector<uint32_t> v_ori;
    v_ori.push_back(100);
    print_vector<uint32_t>(v_ori);

    std::vector<uint32_t> v_ins;
    v_ins.push_back(1);
    v_ins.push_back(2);
    v_ins.push_back(3);
    print_vector<uint32_t>(v_ins);

    v_ori.insert(v_ori.end(), v_ins.begin(), v_ins.end());
    print_vector<uint32_t>(v_ori);

    std::vector<uint32_t> v_ins1;
    v_ins1.push_back(5);
    v_ins1.push_back(6);
    v_ins1.push_back(7);
    print_vector<uint32_t>(v_ins1);
    v_ori.insert(v_ori.end(), v_ins1.begin(), v_ins1.end());
    print_vector<uint32_t>(v_ori);

    return 0;
}
