#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <stdint.h>

struct forum_id2dir_t {
    int32_t forum_id;
    int32_t forum_father_id;
    std::string dir_name;
};

typedef std::map<int32_t, std::pair<int32_t, std::string> > FormuId2Dir;
typedef std::map<std::string, std::pair<int32_t, int32_t> > FormuDir2Id;
