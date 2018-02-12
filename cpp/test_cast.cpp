#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <exception>
#include <boost/lexical_cast.hpp>

bool calc_baiduid(
        const std::string& baiduid_str, 
        uint32_t& baidu_id_shard,
        uint64_t& baidu_id_high,
        uint64_t& baidu_id_low) {
    baidu_id_high = 0; 
    baidu_id_low = 0; 
    baidu_id_shard = 0; 
    if (baiduid_str.size() != 32) {
        return true;
    }    
    std::string baidu_id_high_str = "0x" + baiduid_str.substr(0, 16); 
    std::string baidu_id_low_str = "0x" + baiduid_str.substr(16, 16); 
    std::cout << baiduid_str << " | " << baidu_id_high_str << " | " << baidu_id_low_str << std::endl; 
/*
    uint64_t baidu_id_high_tmp = 0; 
    uint64_t baidu_id_low_tmp = 0; 
    try {
        baidu_id_high_tmp = boost::lexical_cast<uint64_t>(baidu_id_high_str);
        baidu_id_low_tmp = boost::lexical_cast<uint64_t>(baidu_id_low_str);
    } catch (std::exception& e) {
        std::cout << "lexical_cast error: [" << e.what() << "], baiduid=" <<  baiduid_str << std::endl;
        return true;
    }    
    baidu_id_high = baidu_id_high_tmp;
    baidu_id_low = baidu_id_low_tmp;
*/
    std::stringstream baidu_id_high_ss(baidu_id_high_str);
    baidu_id_high_ss >> std::hex >> baidu_id_high;
    std::stringstream baidu_id_low_ss(baidu_id_low_str);
    baidu_id_low_ss >>std:: hex >> baidu_id_low;
    baidu_id_shard = baidu_id_low % 4097; 
    return true;
}

int main() {
    std::vector<std::string> baiduid_v;
    baiduid_v.push_back("D2C93DF50CCAECB6C439BEF7943CE72F");
    baiduid_v.push_back("E48ED0FCF38A573AA8460616716E65E5");
    baiduid_v.push_back("E0E1DFBD7A1986A86F6CC6531033EE21");
    baiduid_v.push_back("640E8C2A841DF1124A139EE29A82F2C9");
    baiduid_v.push_back("F5BE3CCB13ED479393080945D3B6347A");
    baiduid_v.push_back("2CE8CB760A3F3C8E04D494A18DBC4BAB");
    baiduid_v.push_back("2A7FB990798AAEFECCCCFE56FE27A373");
    baiduid_v.push_back("E48ED0FXX38A573AA8460616716YY5E5");

    uint32_t baidu_id_shard;
    uint64_t baidu_id_high;
    uint64_t baidu_id_low;
    for (size_t i = 0; i < baiduid_v.size(); ++i) {
       if (calc_baiduid(baiduid_v[i], baidu_id_shard, baidu_id_high, baidu_id_low)) {
            std::cout << baidu_id_shard << " | " << baidu_id_high << " | " << baidu_id_low << " | " << std::endl;
       }
    }
    return 0;
}
