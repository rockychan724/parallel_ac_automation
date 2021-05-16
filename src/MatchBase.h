#ifndef PARALLEL_AC_AUTOMATION_MATCHBASE_H
#define PARALLEL_AC_AUTOMATION_MATCHBASE_H

#include <vector>
#include <map>
#include <string>

template<typename CharType> // T is generally char, wchar_t, char16_t or char32_t.
class MatchBase {
public:
    using MyString = std::basic_string<CharType>;

    virtual int Init(const std::map<std::string, std::string> &config, const std::vector<MyString> &keywords) = 0;

    virtual std::map<MyString, std::vector<int>> Search(const MyString &text) = 0;
};

#endif //PARALLEL_AC_AUTOMATION_MATCHBASE_H
