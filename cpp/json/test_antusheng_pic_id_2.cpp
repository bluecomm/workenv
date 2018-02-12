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
            // root[] -> json as string
            std::string root_json_str = it_root->second.get<std::string>("json");
            std::cout << root_json_str << std::endl;
            std::stringstream root_json_ss(root_json_str);
            boost::property_tree::ptree root_json_pt;
            boost::property_tree::read_json<boost::property_tree::ptree>(root_json_ss, root_json_pt);  
            boost::property_tree::ptree root_json_item_pt = root_json_pt.get_child("item");
            std::cout << "item array size: " << root_json_item_pt.size() << std::endl;
            // for each item in "item" array
            for (boost::property_tree::ptree::iterator it_item = root_json_item_pt.begin();
                    it_item != root_json_item_pt.end(); ++it_item) {
                pic_urls.push_back(it_item->second.get<std::string>("url"));
                std::cout << "url: " << it_item->second.get<std::string>("url") << std::endl;
            }

        }
    } catch (boost::property_tree::ptree_error& e) {    
        std::cout << "read_json error: " << e.what() << std::endl;
        return false;   
    }
    return true;
}

int main() {
    const std::string input_json_file = "antusheng_2.txt";
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
