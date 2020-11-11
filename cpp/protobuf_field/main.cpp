#include <iostream>
#include <google/protobuf/descriptor.h>

#include "test_field.pb.h"

//using namespace test_field;

namespace test_field {

void find_field_by_name (const std::string field_str0){
	//const string field_str = "test_field::" + field_str0;
	const std::string field_str = field_str0;
    const google::protobuf::FieldDescriptor* field_name = 
        google::protobuf::DescriptorPool::generated_pool()->FindFieldByName(field_str);
	if (field_name != NULL) {
    		std::cout << field_str  << ": " << std::endl;
		std::cout << "  debug string: " << field_name->DebugString();
		//std::cout << "  type name: " << field_name->type_name() << std::endl;
		//std::cout << "  cpp name: " << field_name->cpp_type_name() << std::endl;
		std::cout << "  field name: " << field_name->name() << std::endl;
		std::cout << "  field full name: " << field_name->full_name() << std::endl;
		std::cout << "  field number: " << field_name->number() << std::endl;
		std::cout << "  proto file name: " << field_name->file()->name() << std::endl;
		std::cout << "  package: " << field_name->file()->package() << std::endl;
	} else {
		std::cout << field_str << ": unknown field name " << std::endl;
	}
}

int do_main() {

    TopMessage m0;
    m0.set_type(M2);

    SubMessage2 *s2 = m0.mutable_s2();

    s2->set_a2(123);
    s2->set_b2(456);
    s2->set_c2(789);
    s2->set_d2("hello");

	SubMessage4 *s4 = m0.add_s4();
	s4->set_a4(441);
	s4->set_b4(442);
	s4->set_c4(443);
	s4 = m0.add_s4();
	s4->set_a4(444);
	s4->set_b4(445);
	s4->set_c4(446);

	m0.add_rf(101);
	m0.add_rf(102);
	m0.add_rf(103);

	std::string sep_str = "------------------";
	std::cout << "m0: " << std::endl
			<< m0.type() << std::endl
			<< sep_str << std::endl
			<< "  " << m0.s2().a2() << std::endl
			<< "  " << m0.s2().b2() << std::endl
			<< "  " << m0.s2().c2() << std::endl
			<< "  " << m0.s2().d2() << std::endl;

	for (int i = 0; i < m0.s4_size(); i++){
		SubMessage4 ss4 = m0.s4(i);
		std::cout
			<< sep_str << std::endl
			 << "  " << ss4.a4() << std::endl
			 << "  " << ss4.b4() << std::endl
			 << "  " << ss4.c4() << std::endl;
	}

	for (int i = 0; i < m0.rf_size(); i++){
		std::cout
			<< sep_str << std::endl
			<< "  " << m0.rf(i) << std::endl;
	}

	std::cout
		<< sep_str << std::endl;
	
	find_field_by_name("TopMessage.type");
	find_field_by_name("TopMessage.s2");
	find_field_by_name("TopMessage.s2.a2");
	find_field_by_name("TopMessage.SubMessage2.a2");
	find_field_by_name("SubMessage2.a2");
	find_field_by_name("TopMessage.s4");
	find_field_by_name("TopMessage.rf");
	find_field_by_name("TopMessage.bbb");
	find_field_by_name("nothing");

    return 0;

}
}

int main() {
	return test_field::do_main();
}
