#ifndef PARALLEL_AC_AUTOMATION_MATCHBASE_H
#define PARALLEL_AC_AUTOMATION_MATCHBASE_H

#include <vector>
#include <map>
#include <string>

template<typename T> // T is std::string or std::wstring
class MatchBase {
public:
    virtual int Init(const std::map<std::string, std::string> &config, const std::vector<T> &keywords) = 0;

    virtual std::map<T, std::vector<int>> Search(const T &text) = 0;
};

#endif //PARALLEL_AC_AUTOMATION_MATCHBASE_H
