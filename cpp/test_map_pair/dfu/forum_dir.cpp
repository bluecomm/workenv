#include "forum_dir.h"

namespace sp {
namespace worker {

static const forum_id2dir_t forum_dir[] = {
    {11, 1, "Hello"},
    {13, 3, "Nihao"},
    {12, 2, "World"}
};

ForumDir::ForumDir() {
    for (int i = 0; i < sizeof(forum_dir) / sizeof(forum_dir[0]); ++i) {
        _id_to_dir[forum_dir[i].forum_id] = std::make_pair(forum_dir[i].forum_father_id, 
                forum_dir[i].dir_name);
        _dir_to_id[forum_dir[i].dir_name] = std::make_pair(forum_dir[i].forum_id, 
                forum_dir[i].forum_father_id);
    }
}

bool ForumDir::get_forum_id(const std::string& forum_dir_str, 
            const std::string& forum_second_dir_str,
            uint32_t& forum_dir_id, 
            uint32_t& forum_second_dir_id) {
    int32_t forum_dir_id_tmp = 0;
    int32_t forum_second_dir_id_tmp = 0;

    if (forum_dir_str.empty() || forum_second_dir_str.empty()) {
        // forum_dir_str is empty
        return false;
    }
    std::map<std::string, std::pair<int32_t, int32_t> >::iterator iter1 = 
            _dir_to_id.find(forum_dir_str);
    std::map<std::string, std::pair<int32_t, int32_t> >::iterator iter2 = 
            _dir_to_id.find(forum_second_dir_str);
    if (iter1 == _dir_to_id.end() || iter2 == _dir_to_id.end()) {
        // forum_dir not found
        return false;
    }

    forum_dir_id_tmp = iter1->second.first;
    forum_second_dir_id_tmp = iter2->second.first;

    std::map<int32_t, std::pair<int32_t, std::string> >::iterator iter3 = 
            _id_to_dir.find(forum_second_dir_id_tmp);
    if (iter3 == _id_to_dir.end()) {
        // forum id not found(never run here)
        return false;
    }

    if (forum_dir_id_tmp != iter3->second.first 
                || forum_dir_id_tmp != forum_second_dir_id_tmp) {
        // forum_second_dir_id's father is not forum_dir_id_tmp
        return false;
    }

    forum_dir_id = (uint32_t) forum_dir_id_tmp;
    forum_second_dir_id = (uint32_t) forum_second_dir_id_tmp;

    return true;
}

void ForumDir::output() {
    for (ForumId2Dir::iterator iter = _id_to_dir.begin(); iter != _id_to_dir.end(); iter++) {
        std::cout << iter->first << "\t" << iter->second.first << "\t" << 
                iter->second.second << std::endl;
    }
    for (ForumDir2Id::iterator iter = _dir_to_id.begin(); iter != _dir_to_id.end(); iter++) {
        std::cout << iter->first << "\t" << iter->second.first << "\t" << 
                iter->second.second << std::endl;
    }
}

}  // namespace worker
}  // namespace sp

int main () {
    sp::worker::ForumDir::get_instance()->output();
    return 0;
}

