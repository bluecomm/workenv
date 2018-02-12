#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <stdint.h>

namespace sp {
namespace worker {

struct forum_id2dir_t {
    int32_t forum_id;
    int32_t forum_father_id;
    std::string dir_name;
};

typedef std::map<int32_t, std::pair<int32_t, std::string> > ForumId2Dir;
typedef std::map<std::string, std::pair<int32_t, int32_t> > ForumDir2Id;

class ForumDir {
public:
    static ForumDir *get_instance() {
        ForumDir *p_instance = NULL;
        try {
            static ForumDir s_instance;
            p_instance = &s_instance;
        } catch(...) {
            //WORKER_FATAL_LOG("failed to create singleton. [classname='#classname']");
            p_instance = NULL;
        }
        return p_instance;
    }

    ~ForumDir() {};

    bool get_forum_id(const std::string& forum_dir_str, 
            const std::string& forum_second_dir_str,
            uint32_t& forum_dir_id, 
            uint32_t& forum_second_dir_id);

    void output();

private:
    ForumDir();
    ForumId2Dir _id_to_dir;
    ForumDir2Id _dir_to_id;
};

}  // namespace worker
}  // namespace sp
