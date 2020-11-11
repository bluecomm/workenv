/**
 * 20160909
 */
#include <iostream>
#include <sstream> 
#include <vector>
#include <string>
#include <utility>
#include "base64.h"
#include "pair.pb.h"
#include "google/protobuf/message_lite.h"
#include "google/protobuf/message.h"

typedef int32_t (*EncodeDecodeFuc)(const std::string &, std::string &);

std::string integer2string(uint64_t num) {                                                               
        std::ostringstream sstream;                                                                          
        sstream << num;                                                                                      
        return sstream.str();                                                                                
}  

std::string make_text_string(const std::vector<std::pair<int, int> >& pairs) {
    std::string out_str;
    std::vector<std::pair<int, int> >::const_iterator ite = pairs.begin();
    for(;ite != pairs.end(); ++ite) {
       uint32_t source_id = ite->first;
       uint32_t covert_type = ite->second;
       if (ite == pairs.begin()) {
           out_str = integer2string(source_id) + "|" + integer2string(covert_type);
       } else {
           out_str += "#" + integer2string(source_id) + "|" + integer2string(covert_type);
       }
    }
    return out_str;
}

std::string make_pb_string(const std::vector<std::pair<int, int> >& pairs) {
    std::string out_str;
    CvtPairs cvt_pairs;
    EncodeDecodeFuc encode_func = encode;
    //EncodeDecodeFuc decode_func = decode;
    std::vector<std::pair<int, int> >::const_iterator ite = pairs.begin();
    for(;ite != pairs.end(); ++ite) {
       uint32_t source_id = ite->first;
       uint32_t convert_type = ite->second;
       CvtPair* cp = cvt_pairs.add_cvt_pair();
       cp->set_source_id(source_id);
       cp->set_convert_type(convert_type);
    }
    std::string out_str_uncode;
    cvt_pairs.SerializeToString(&out_str_uncode);
    encode_func(out_str_uncode, out_str);

    return out_str;
}

int main(){
    std::vector<std::pair<int, int> > pairs;
    pairs.push_back(std::make_pair(1, 17));
    pairs.push_back(std::make_pair(2, 13));
    pairs.push_back(std::make_pair(3, 5));
    pairs.push_back(std::make_pair(4, 9));
    pairs.push_back(std::make_pair(5, 16));

    std::string out_text_string = make_text_string(pairs);
    std::string out_pb_string = make_pb_string(pairs);

    std::cout << "Text string(" << out_text_string.size() << "):" << out_text_string << std::endl; 
    std::cout << "  Pb string(" << out_pb_string.size() << "):" << out_pb_string << std::endl; 

    return 0;
}
