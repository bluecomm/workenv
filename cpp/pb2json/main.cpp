#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdio.h>
#include "pack.pb.h"
#include "pb2json.h"

int main() {
    NewStyle ns;
    ns.set_planid(1001);
    ns.set_unitid(2001);
    ns.add_mtid(2062);
    ns.add_mtid(2892);
    ns.add_mtid(3016);
    ns.mutable_it()->set_in_a(11);
    ns.mutable_it()->set_in_b(12);
    ns.mutable_it()->set_et1(InnerType::Type1);
    ns.mutable_it()->set_et2(InnerType::Type2);
    ns.set_url("baidu.com");

    std::cout << ns.ShortDebugString() << std::endl;

    // pb 2 json
    nlohmann::json json;
    Pb2Json::Message2Json(ns, json);
    // output
    std::cout << json << std::endl;
    // output
    std::ostringstream sstream;
    sstream << json;
    std::string out_json = sstream.str();
    std::cout << out_json <<std::endl;

    NewStyle ns1;
    nlohmann::json json1;
    json1["planid"] = 1002;
    json1["unitid"] = 2001;
    json1["mtid"] = {997, 998, 999};
    json1["it"]["in_a"] = 111;
    if (!Pb2Json::Json2Message(json1, ns1)) {
        std::cout << "Json2Message error" << std::endl;
        return 1;
    }
    std::cout << ns1.mtid_size() << std::endl;
    std::cout << ns1.it().in_a() << std::endl;

    return 0;
}
