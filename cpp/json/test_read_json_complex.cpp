#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <exception>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

bool parse_complex_json(const std::string& json_str, std::vector<std::string>& pic_urls) {
    std::stringstream root_ss(json_str);
    boost::property_tree::ptree root_pt; 
    try{    
        boost::property_tree::read_json<boost::property_tree::ptree>(root_ss, root_pt);  
    } catch(boost::property_tree::ptree_error& e) {    
        std::cout << "read_json error: " << e.what() << std::endl;
        return false;   
    }

    try {
        boost::property_tree::ptree reserved_pt = root_pt.get_child("reserved");  // get_child得到数组对象  
        std::string image_source_str = reserved_pt.get<std::string>("image_source");  // get_child得到数组对象  
        if (image_source_str.compare("vcg") != 0) { 
            return false;
        }
        std::cout << "image_source: " << "vcg" << std::endl;
    } catch (boost::property_tree::ptree_error& e) {    
        std::cout << "read_json error: " << e.what() << std::endl;
        return false;   
    }    
  
    try{  
        boost::property_tree::ptree item_array_pt = root_pt.get_child("item");  // get_child得到数组对象  
        BOOST_FOREACH(boost::property_tree::ptree::value_type &v, item_array_pt) { // 遍历数组  
            pic_urls.push_back(v.second.get<std::string>("url"));
            std::cout << "url: " << v.second.get<std::string>("url") << std::endl;
        }    
    } catch (boost::property_tree::ptree_error& e) {    
        std::cout << "read_json error: " << e.what() << std::endl;
        return false;   
    }    
    return true;
}

int main() {
    const std::string input_json_file = "antusheng.txt";
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
