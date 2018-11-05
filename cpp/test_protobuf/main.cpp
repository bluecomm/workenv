#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdio.h>
#include "pack.pb.h"
#include "updater_pusher.pb.h"

int main() {
    std::cout << "***********************************************************" << std::endl;
    NewStyle ns;
    ns.set_unitid(202457426);
    if (ns.has_planid()) {
        std::cout << "planid: filled" << std::endl;
    } 
    if (ns.has_unitid()) {
        std::cout << "unitid: filled" << std::endl;
    }
    if (ns.has_cmatch()) {
        std::cout << "cmatch_id: filled" << std::endl;
    }
    std::cout << "planid: " << ns.planid() << std::endl;
    std::cout << "unitid: " << ns.unitid() << std::endl;
    std::cout << "cmatch: " << ns.cmatch() << std::endl;

    std::cout << "***********************************************************" << std::endl;
    sp::worker::OLAPUpdateShortMessage ousm;
    ousm.set_type("hwis");
    ousm.mutable_hwis()->set_date(1031000);
    ousm.mutable_hwis()->set_hour(20);
    ousm.mutable_hwis()->set_user_id(1301000);
    ousm.mutable_hwis()->set_plan_id(12345678);
    ousm.mutable_hwis()->set_unit_id(23456789);
    ousm.mutable_hwis()->set_winfo_id(103100000000);
    ousm.mutable_hwis()->set_word_id(103100000001);
    ousm.mutable_hwis()->set_idea_id(103100000002);
    ousm.mutable_hwis()->set_type(1);
    ousm.mutable_hwis()->set_cmatch_id(222);
    ousm.mutable_hwis()->set_mtag(1);
    ousm.mutable_hwis()->set_wmatch_id(63);
    ousm.mutable_hwis()->set_lab_id(101);
    ousm.mutable_hwis()->set_flow_type(3);

    sp::worker::OLAPUpdateMessage oum;
    oum.set_type("hwis");
    oum.mutable_hwis()->set_date(1031000);
    oum.mutable_hwis()->set_hour(20);
    oum.mutable_hwis()->set_user_id(1301000);
    oum.mutable_hwis()->set_plan_id(12345678);
    oum.mutable_hwis()->set_unit_id(23456789);
    oum.mutable_hwis()->set_winfo_id(103100000000);
    oum.mutable_hwis()->set_word_id(103100000001);
    oum.mutable_hwis()->set_idea_id(103100000002);
    oum.mutable_hwis()->set_type(1);
    oum.mutable_hwis()->set_cmatch_id(222);
    oum.mutable_hwis()->set_mtag(1);
    oum.mutable_hwis()->set_wmatch_id(63);
    oum.mutable_hwis()->set_lab_id(101);
    oum.mutable_hwis()->set_flow_type(3);

    std::cout << "sizeof(OLAPUpdateShortMessage): " << sizeof(ousm) << "/"
        << sizeof(sp::worker::OLAPUpdateShortMessage) << "/"
        << ousm.ShortDebugString().size() << std::endl;
    std::cout << ousm.ShortDebugString() << std::endl;
    std::cout << "sizeof(OLAPUpdateMessage): " << sizeof(oum) << "/"
        << sizeof(sp::worker::OLAPUpdateMessage) << "/"
        << oum.ShortDebugString().size() << std::endl;
    std::cout << oum.ShortDebugString() << std::endl;

    sp::worker::OLAPUpdateShortMessage* a = new sp::worker::OLAPUpdateShortMessage();
    sp::worker::OLAPUpdateMessage* b = new sp::worker::OLAPUpdateMessage();
    std::cout << sizeof(a) << " " << sizeof(*a) << std::endl;
    std::cout << sizeof(b) << " " << sizeof(*b) << std::endl;
    std::cout << "***********************************************************" << std::endl;
    sp::worker::DataQualityUpdateMessage* dq = new sp::worker::DataQualityUpdateMessage();
    dq->set_type("mdqs");
    dq->mutable_mdqs()->set_date(1031011);
    dq->mutable_mdqs()->set_hour(21);
    dq->mutable_mdqs()->set_minute(21);
    dq->mutable_mdqs()->set_check_type(1);
    dq->mutable_mdqs()->set_group_name("test group name");
    std::cout << "DataQualityUpdateMessage" << dq->ShortDebugString() << std::endl;
    std::string a_str;
    dq->SerializeToString(&a_str);
    sp::worker::OLAPUpdateMessage ou;
    if (!ou.ParseFromString(a_str)) {
        std::cout << "parse error" << std::endl;
    } else {
        std::cout << "OLAPUpdateMessage" << ou.ShortDebugString() << std::endl;
    }
    std::cout << "***********************************************************" << std::endl;
}
