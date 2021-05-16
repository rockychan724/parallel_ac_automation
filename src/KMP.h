#ifndef PARALLEL_AC_AUTOMATION_KMP_H
#define PARALLEL_AC_AUTOMATION_KMP_H

#include "MatchBase.h"
#include <iostream>

template<typename CharType>
class KMP : public MatchBase<CharType> {
public:
    using MyString = std::basic_string<CharType>;

    int Init(const std::map<std::string, std::string> &config, const std::vector<MyString> &_keywords) {
        // Check config parameter
        auto it = config.find("use_optimize");
        if (it == config.end()) {
            std::cout << "Parameter \"use_optimize\" is required in class KMP!!!\n";
            return 0;
        }
        int use_optimize = std::stoi(it->second);

        // Get next arrays
        this->keywords = _keywords;
        this->nexts.resize(_keywords.size());
        for (size_t i = 0; i < _keywords.size(); i++) {
            if (use_optimize == 0)
                this->nexts[i] = KMP::GetNext(_keywords[i]);
            else
                this->nexts[i] = KMP::GetNextOptimized(_keywords[i]);
        }

        return 1;
    }

    std::map<MyString, std::vector<int>> Search(const MyString &text) {
        std::map<MyString, std::vector<int>> res;
        for (size_t t = 0; t < this->keywords.size(); t++) {
            MyString pattern = this->keywords[t];
            std::vector<int> next = this->nexts[t];
            int n = text.length(), m = pattern.length();
            int i = 0, j = 0;
            while (i < n && j < m) {
                if (j == -1 || text[i] == pattern[j]) {
                    i++, j++;
                } else {
                    j = next[j];
                }
            }
            if (j == m) {
                if (res.find(pattern) == res.end())
                    res.insert({pattern, {i - j}});
                else
                    res[pattern].push_back(i - j);
            }
        }
        return res;
    }

protected:
    std::vector<MyString> keywords;
    std::vector<std::vector<int>> nexts;

    static std::vector<int> GetNext(const MyString &pattern) {
        std::vector<int> next(pattern.length());
        next[0] = -1;
        int len = pattern.length();
        int j = 0, k = -1;
        while (j < len - 1) {
            if (k == -1 || pattern[j] == pattern[k]) {
                j++, k++;
                next[j] = k;
            } else {
                k = next[k];
            }
        }
        return next;
    }

    static std::vector<int> GetNextOptimized(const MyString &pattern) {
        std::vector<int> next(pattern.length());
        next[0] = -1;
        int len = pattern.length();
        int j = 0, k = -1;
        while (j < len - 1) {
            if (k == -1 || pattern[j] == pattern[k]) {
                j++, k++;
                if (pattern[j] != pattern[k])
                    next[j] = k;
                else {
                    next[j] = next[k];
                }
            } else {
                k = next[k];
            }
        }
        return next;
    }
};

#endif //PARALLEL_AC_AUTOMATION_KMP_H
