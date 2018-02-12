#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <exception>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

// parse json to <k,v> pairs
bool parse_simple_json(const std::string& json_str,
        std::map<std::string, std::string>& result) {
    if (json_str.empty()) {
        std::cout << "json_str is empty" << std::endl;
        return false;
    }
    std::cout << "json string: " << json_str << std::endl;
    boost::property_tree::ptree pt;  
    std::stringstream ss(json_str);
    try {
        boost::property_tree::read_json<boost::property_tree::ptree>(ss, pt); 
    } catch (std::exception& e) {
        std::cout << "read_json[" << json_str << "] failed: " << e.what() << std::endl;
        return false;
    }    
    std::string json_key;
    std::string json_value;
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt) {
        json_key = v.first;
        json_value = v.second.data();
        result[json_key] = json_value;
    }    
    return true;
}

int main() {
    std::string json_str = "{\"1\":5,\"2\":10,\"3\":20, \"4\":30}";
    boost::property_tree::ptree pt;
    std::stringstream ss(json_str);
    boost::property_tree::read_json<boost::property_tree::ptree>(ss, pt);
    int code = pt.get<int>("1");
    std::cout << "1: " << code << std::endl;
    code = pt.get<int>("2");
    std::cout << "2: " << code << std::endl;
    code = pt.get<int>("3");
    std::cout << "3: " << code << std::endl;
    std::cout << "****************************************" << std::endl;

    std::map<std::string, std::string> result;
    result.clear();
    parse_simple_json(json_str, result);
    for (std::map<std::string, std::string>::iterator iter = result.begin();
            iter != result.end(); ++iter) {
        std::cout << iter->first << ": " << iter->second << std::endl;
    }
    std::cout << "****************************************" << std::endl;

    json_str = "{\"1a\":5x,\"2b\":10xx,\"3c\":15xxx}";
    result.clear();
    parse_simple_json(json_str, result);
    for (std::map<std::string, std::string>::iterator iter = result.begin();
            iter != result.end(); ++iter) {
        std::cout << iter->first << ": " << iter->second << std::endl;
    }
    std::cout << "****************************************" << std::endl;

    json_str = "{\"1a\":\"5x\",\"2b\":\"10xx\",\"3c\":\"15xxx\"}";
    result.clear();
    parse_simple_json(json_str, result);
    for (std::map<std::string, std::string>::iterator iter = result.begin();
            iter != result.end(); ++iter) {
        std::cout << iter->first << ": " << iter->second << std::endl;
    }

    return 0;
}
