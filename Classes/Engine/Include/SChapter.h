#ifndef SCHAPTER_H
#define SCHAPTER_H
#include <string>
#include <map>
#include <vector>

struct SChapter {
    std::string mName;
    std::map<int, std::vector<std::string> > mScenes;
};
#endif // SChapter
