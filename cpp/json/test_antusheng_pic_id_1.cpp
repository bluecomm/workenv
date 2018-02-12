#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <exception>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <stdint.h>

bool parse_complex_json(const std::string& json_str, std::vector<std::string>& pic_urls) {
    std::stringstream root_ss(json_str);
    boost::property_tree::ptree root_pt; 
    // get root
    try{    
        boost::property_tree::read_json<boost::property_tree::ptree>(root_ss, root_pt);  
    } catch(boost::property_tree::ptree_error& e) {    
        std::cout << "read_json error: " << e.what() << std::endl;
        return false;   
    }

    std::cout << "root array size: " << root_pt.size() << std::endl;

    try{  
        // for each item in root array
        for (boost::property_tree::ptree::iterator it_root = root_pt.begin();
                it_root != root_pt.end(); ++it_root) {
            // root[] -> mt
            uint32_t root_mt = it_root->second.get<uint32_t>("mt");
            std::cout << "mt: " << root_mt << std::endl;
            // root[] -> json
            boost::property_tree::ptree root_json_pt = it_root->second.get_child("json");
            // root[] -> json -> user_icon
            std::string root_json_user_icon = root_json_pt.get<std::string>("user_icon");
            std::cout << "user_icon: " << root_json_user_icon << std::endl;
            // root[] -> json -> reserved -> image_source
            boost::property_tree::ptree root_json_reserved_pt = root_json_pt.get_child("reserved");
            std::string image_source = root_json_reserved_pt.get<std::string>("image_source");
            std::cout << "image_source: " << image_source << std::endl;
            // root[] -> json -> item
            boost::property_tree::ptree root_json_item_pt = root_json_pt.get_child("item");
            std::cout << "item array size: " << root_json_item_pt.size() << std::endl;
            // for each item in "item" array
            for (boost::property_tree::ptree::iterator it_item = root_json_item_pt.begin();
                    it_item != root_json_item_pt.end(); ++it_item) {
                pic_urls.push_back(it_item->second.get<std::string>("url"));
                std::cout << "url: " << it_item->second.get<std::string>("url") << std::endl;
            }
//            BOOST_FOREACH(boost::property_tree::ptree::value_type &v_item, item_array_pt) {
//                pic_urls.push_back(v_item.second.get<std::string>("url"));
//                std::cout << "url: " << v_item.second.get<std::string>("url") << std::endl;
//            }    
        }
    } catch (boost::property_tree::ptree_error& e) {    
        std::cout << "read_json error: " << e.what() << std::endl;
        return false;   
    }
    return true;
}

int main() {
    const std::string input_json_file = "antusheng_1.txt";
    std::ifstream if_json(input_json_file.c_str());
    std::string str_json((std::istreambuf_iterator<char>(if_json)),
            std::istreambuf_iterator<char>());
    std::cout << "json string: " << std::endl << str_json << std::endl;
    std::vector<std::string> pic_urls;
    if (!parse_complex_json(str_json, pic_urls)) {
        std::cout << "parse failed!" << std::endl;
    } else {
        std::cout << "parse success, size: " << pic_urls.size() << std::endl;
    }
    return 0;
}
