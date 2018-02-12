#include "forum_dir.h"

forum_id2dir_t forum_dir[] = {
{11, 1, "Hello"},
{12, 2, "World"}
};


int main () {
    FormuId2Dir f2d;
    FormuDir2Id d2f;
    for (int i = 0; i < sizeof(forum_dir) / sizeof(forum_dir[0]); ++i) {
        f2d[forum_dir[i].forum_id] = std::make_pair(forum_dir[i].forum_father_id, forum_dir[i].dir_name);
        d2f[forum_dir[i].dir_name] = std::make_pair(forum_dir[i].forum_id, forum_dir[i].forum_father_id);
    }
    for (FormuId2Dir::iterator iter = f2d.begin(); iter != f2d.end(); iter++) {
        std::cout << iter->first << "\t" << iter->second.first << "\t" << iter->second.second << std::endl;
    }
    for (FormuDir2Id::iterator iter = d2f.begin(); iter != d2f.end(); iter++) {
        std::cout << iter->first << "\t" << iter->second.first << "\t" << iter->second.second << std::endl;
    }
    return 0;
}
