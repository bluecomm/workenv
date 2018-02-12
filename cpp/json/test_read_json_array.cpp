#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <exception>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

// parse json with arrays
int parse_json_array(){
    std::string str =
    "{"
        "\"code\":0,"
        "\"images\":"
            "[{"
                "\"url\":\"fmn057/20111221/1130/head_kJoO_05d9000251de125c.jpg\""
            "},"
            "{"
                "\"url\":\"fmn057/20111221/1130/original_kJoO_05d9000251de125c.jpg\""
            "}]"
    "}";  
  
    std::stringstream ss(str);  
    boost::property_tree::ptree pt;  
    try{      
        boost::property_tree::read_json<boost::property_tree::ptree>(ss, pt);  
    } catch(boost::property_tree::ptree_error & e) {  
        return 1;   
    }  
  
    try{  
        int code = pt.get<int>("code");   // 得到"code"的value  
        boost::property_tree::ptree image_array = pt.get_child("images");  // get_child得到数组对象  
        // 遍历数组  
        BOOST_FOREACH(boost::property_tree::ptree::value_type &v, image_array) {
            // std::cout << v.second.data() << std::endl;
            std::stringstream s;
            write_json(s, v.second);  
            std::string image_item = s.str();  
            std::cout << image_item << std::endl;
        }  
    } catch (boost::property_tree::ptree_error & e) {  
        std::cout << "process error: " <<  e.what() << std::endl;
        return 2;  
    }  
    try{  
        boost::property_tree::ptree image_array = pt.get_child("noimages");  // get_child得到数组对象  
        // 遍历数组  
        BOOST_FOREACH(boost::property_tree::ptree::value_type &v, image_array) {
            // std::cout << v.second.data() << std::endl;
            std::stringstream s;
            write_json(s, v.second);  
            std::string image_item = s.str();  
            std::cout << image_item << std::endl;
        }  
    } catch (boost::property_tree::ptree_error & e) {  
        std::cout << "process error: " <<  e.what() << std::endl;
        return 2;  
    }  
  return 0;  
}  

int main() {
    parse_json_array();
    return 0;
}
