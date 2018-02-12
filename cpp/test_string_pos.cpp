#include <string>
#include <iostream>
#include <vector>
#include <boost/lexical_cast.hpp>

// http://blue.baidu.com/nichang/image/public//vcg/ac608a/1008859207.jpg@c_1,x_0,y_0
// -> 1008859207
bool get_antusheng_single_pic_id(std::vector<std::string> pic_urls,
        std::vector<uint64_t>& pic_ids) {
    for (size_t indx = 0; indx < pic_urls.size(); ++ indx) {
        std::string pic_url = pic_urls[indx];
        size_t pos_2 = pic_url.find(".jpg@");
        if (pos_2 == std::string::npos) {
            continue;
        }
        std::string str_part1 = pic_url.substr(0, pos_2);
        size_t pos_1 = str_part1.find_last_of('/');
        if (pos_1 == std::string::npos) {
            continue;
        }
        std::string pic_id_str = str_part1.substr(pos_1 + 1);
        try {
            uint64_t pic_id = boost::lexical_cast<uint64_t>(pic_id_str);
            pic_ids.push_back(pic_id);
        } catch (...) {
            std::cout << "cast error: " << pic_id_str.c_str() << std::endl;
            continue;
        }
    }
    return true;
}

int main() {
    std::vector<std::string> pic_urls;
    std::vector<uint64_t> pic_ids;
    pic_urls.push_back("http://blue.baidu.com/nichang/image/public//vcg/ac608a/1008859207.jpg@c_1,x_0,y_0");
    pic_urls.push_back("http://blue.baidu.com/nichang/image/public//vcg/3ea129/812193788.jpg@c_1,x_0,y_0");
    pic_urls.push_back("http://blue.baidu.com/nichang/image/public/fotomore/4b6c2c/35775578.jpg@c_1,x_0,y_0");
    if (!get_antusheng_single_pic_id(pic_urls, pic_ids)) {
        std::cout << "get pic id error" << std::endl;
        return 0;
    }

    for (size_t i = 0; i < pic_ids.size(); ++i) {
        std::cout << "pic_id: " << pic_ids[i] << std::endl;
    }

    return 0;
}
