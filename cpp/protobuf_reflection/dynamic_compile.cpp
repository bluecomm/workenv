#include <iostream>
#include <google/protobuf/compiler/importer.h> // Importer DiskSourceTree MultiFileErrorCollector
#include <google/protobuf/dynamic_message.h> // DynamicMessageFactory 
#include <google/protobuf/descriptor.h> // Desctriptor FiledDescriptor ... all descriptor
#include <google/protobuf/message.h> // Message Reflection MessageFactory

// https://www.jianshu.com/p/e692a6a2f78e
//
//namespace foo {

int do_main(){
    google::protobuf::compiler::DiskSourceTree sourceTree;
    //look up .proto file in current directory
    sourceTree.MapPath("", "./");
    google::protobuf::compiler::Importer importer(&sourceTree, NULL);
    //runtime compile foo.proto
    importer.Import("foo.proto");

    const google::protobuf::Descriptor *descriptor = importer.pool()->FindMessageTypeByName("foo::Pair");
    if (descriptor == NULL){
        std::cout << "descriptor is NULL !" << std::endl;
        return -1;
    }

    std::cout << descriptor->DebugString() << std::endl;

    // build a dynamic message by "Pair" proto
    google::protobuf::DynamicMessageFactory factory;
    const google::protobuf::Message *message = factory.GetPrototype(descriptor);
    // create a real instance of "Pair"
    google::protobuf::Message *pair = message->New();

    // write the "Pair" instance by reflection
    const google::protobuf::Reflection *reflection = pair->GetReflection();

    const google::protobuf::FieldDescriptor *field = NULL;
    field = descriptor->FindFieldByName("key");
    reflection->SetString(pair, field, "my key");
    field = descriptor->FindFieldByName("value");
    reflection->SetUInt32(pair, field, 1111U);

    std::cout << pair->DebugString() << std::endl;

    delete pair;

    return 0;
}

//} //namespace foo

int main(int argc, char **argv){
    //return foo::do_main();
    do_main();
}

