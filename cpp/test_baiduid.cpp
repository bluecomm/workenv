#include <iostream>
#include <string>

bool is_valid_baiduid(const std::string& baiduid) {                                                      
    if (baiduid.size() != 32) {                                                                          
        return false;                                                                                    
    }                                                                                                    
    for (size_t i = 0; i < baiduid.size(); ++i) {                                                        
        // 0~9, A~F, a~f                                                                                 
        // 48~57, 65~70, 97~102                                                                          
        if ((baiduid[i] < '0')                                                                           
                || (baiduid[i] > '9' && baiduid[i] < 'A')                                                
                || (baiduid[i] > 'F' && baiduid[i] < 'a' )                                               
                || (baiduid[i] > 'f')) {                                                                 
            return false;                                                                                
        }                                                                                                
    }                                                                                                    
    return true;                                                                                         
} 

int main() {
    std::string baiduids[] = {
        "792e2e2cbed7a8d10cccc2fe5b9679d8",
        "792E2E2CBED7A8D10CCCC2FE5B9679D8",
        "00000000000000000000000000000000",
        "D32FCE24CA0C674C70C5A717F7402C39",
        "24CA0C674C70C5A717F7402C39",
        "D32FCE24CA0C674C70C5A717F7402X39",
        "10.42.43.13.56751530145733668",
        "\"6A0F0CF4388F7F48A668A264F17D594",
        "QQ1234"
    };
    for (size_t i = 0; i < 8U; ++i) {
        if (is_valid_baiduid(baiduids[i])) {
            std::cout << "valid: \t" << baiduids[i] << std::endl;
        } else {
            std::cout << "invalid: \t" << baiduids[i] << std::endl;
        }
    }
    return 0;
}
