#include <vector>
#include <string>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/join.hpp>
#include <google/protobuf/message.h> 
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/dynamic_message.h>

#include "foo.pb.h"

bool serialize2csv(const google::protobuf::Message& msg, std::string& output) {
    output.clear();

    // record format:
    // field1_value\tfield2_value\t...fieldn_value\n
    std::vector<std::string> field_values;
    const google::protobuf::Reflection* ref = msg.GetReflection();
    if (ref == NULL) {
        std::cout << "ref is null ptr" << std::endl;
        return false;
    }

    std::vector<const google::protobuf::FieldDescriptor*> fields;
    ref->ListFields(msg, &fields);
    for (size_t i = 0; i < fields.size(); i++) {
        const google::protobuf::FieldDescriptor* fdesc = fields[i];
        if (fdesc == NULL) {
            std::cout << "fdesc is null ptr" << std::endl;
        }

        if (fdesc->is_repeated()) {
            std::cout << "not support repeated field[" << fdesc->name().c_str() << "]" << std::endl;
            return false;
        }
/*
field->cpp_type()      T
CPPTYPE_INT32        int32
CPPTYPE_UINT32       uint32
CPPTYPE_INT64        int64
CPPTYPE_UINT64       uint64
CPPTYPE_DOUBLE       double
CPPTYPE_FLOAT        float
CPPTYPE_BOOL         bool
CPPTYPE_ENUM         generated enum type or int32
CPPTYPE_STRING       std::string
CPPTYPE_MESSAGE      generated message type or google::protobuf::Message
 * */
        switch (fdesc->cpp_type()) {
#ifndef CSVSERIALIZER_PRINT
#define CSVSERIALIZER_PRINT(type, method) \
            case google::protobuf::FieldDescriptor::CPPTYPE_##type: \
                std::cout << fdesc->full_name().c_str() << ": " << ref->Get##method(msg, fdesc) << std::endl; \
                field_values.push_back(\
                        boost::lexical_cast<std::string>(ref->Get##method(msg, fdesc))); \
                break;
#endif
            CSVSERIALIZER_PRINT(INT32, Int32)
            CSVSERIALIZER_PRINT(INT64, Int64)
            CSVSERIALIZER_PRINT(UINT32, UInt32)
            CSVSERIALIZER_PRINT(UINT64, UInt64)
            CSVSERIALIZER_PRINT(FLOAT, Float)
            CSVSERIALIZER_PRINT(DOUBLE, Double)
            CSVSERIALIZER_PRINT(BOOL, Bool)
            case google::protobuf::FieldDescriptor::CPPTYPE_STRING: {
                std::string scratch;
                std::cout << fdesc->full_name().c_str() << ": " << ref->GetStringReference(msg, fdesc, &scratch) << std::endl;
                field_values.push_back(ref->GetStringReference(msg, fdesc, &scratch));
                break;
            }
            case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE: {
                std::string sub_output;
                bool ret = serialize2csv(ref->GetMessage(msg, fdesc), sub_output);
                if (ret == false) {
                    return false;
                }
                field_values.push_back(sub_output);
                break;
            }
            default:
               std::cout << "protobuf type " << fdesc->cpp_type() << " not support" << std::endl;
                return false;
        }
    }

    output = boost::join(field_values, "\t");
    return true;
}

int main() {
    foo::AggMessage agg;
    agg.set_key1(4096U);
    agg.set_key2(8192U);
    agg.set_key3("213");
    agg.set_key4("214");
    agg.set_value6(256);
    std::string output;
    serialize2csv(agg, output);
    std::cout << output << std::endl;
    return 0;
}
