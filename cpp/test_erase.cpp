#include <vector>
#include <iostream>

int main() {
    std::vector<int> v_int = {1, 2, 3, 4, 5};
    std::cout << "size: " << v_int.size() << std::endl;
    for (std::vector<int>::iterator ite = v_int.begin(); ite != v_int.end(); ++ite) {
        if (*ite == 3) {
            //v_int.erase(ite);
            ite = v_int.erase(ite);
            --ite;
            std::cout << "erase 3" << std::endl;
            continue;
        }
        std::cout << "data: " << *ite << std::endl;
    }
    std::cout << "size: " << v_int.size() << std::endl;
    return 0;
}
