#ifndef PARALLEL_AC_AUTOMATION_KMP_H
#define PARALLEL_AC_AUTOMATION_KMP_H

#include "MatchBase.h"
#include <algorithm>

template<typename CharType>
class KMP : public MatchBase<CharType> {
public:
    using MyString = std::basic_string<CharType>;

    int Init(const std::map<std::string, std::string> &config, const std::vector<MyString> &_keywords) {
        if (this->CheckConfig(config) == 0)
            return 0;

        this->keywords = _keywords;
        this->nexts.resize(this->keywords.size());
        // Get next arrays
        for (size_t i = 0; i < this->keywords.size(); i++) {
            if (!this->case_sensitive)
                std::transform(this->keywords[i].begin(), this->keywords[i].end(), this->keywords[i].begin(),
                               ::tolower);
            if (this->use_optimize == 0)
                this->nexts[i] = KMP::GetNext(this->keywords[i]);
            else
                this->nexts[i] = KMP::GetNextOptimized(this->keywords[i]);
        }

        return 1;
    }

    std::map<MyString, std::vector<int>> Search(const MyString &_text) {
        MyString text = _text;
        if (!this->case_sensitive)
            std::transform(text.begin(), text.end(), text.begin(), ::tolower);
        std::map<MyString, std::vector<int>> res;
        for (size_t t = 0; t < this->keywords.size(); t++) {
            this->SearchAll(text, this->keywords[t], this->nexts[t], res);
        }
        return res;
    }

protected:
    int CheckConfig(const std::map<std::string, std::string> &config) {
        // Check config parameter
        auto it = config.find("case_sensitive");
        if (it == config.end()) {
            std::cout << "Parameter \"case_sensitive\" is required in class KMP!!!\n";
            return 0;
        }
        this->case_sensitive = std::stoi(it->second);

        it = config.find("use_optimize");
        if (it == config.end()) {
            std::cout << "Parameter \"use_optimize\" is required in class KMP!!!\n";
            return 0;
        }
        this->use_optimize = std::stoi(it->second);
        return 1;
    }

private:
    std::vector<MyString> keywords;
    std::vector<std::vector<int>> nexts;
    bool case_sensitive;
    bool use_optimize;

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

    void SearchOnce(const MyString &text, const MyString &pattern, const std::vector<int> &next,
                    std::map<MyString, std::vector<int>> &res) {
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

    void SearchAll(const MyString &text, const MyString &pattern, const std::vector<int> &next,
                   std::map<MyString, std::vector<int>> &res) {
        int n = text.length(), m = pattern.length();
        int i = 0, j = 0;
        while (i < n) {
            if (j == -1 || text[i] == pattern[j]) {
                i++, j++;
            } else {
                j = next[j];
            }

            if (j == m) {
                if (res.find(pattern) == res.end())
                    res.insert({pattern, {i - j}});
                else
                    res[pattern].push_back(i - j);
                j = 0; // 找到之后将j归零，继续找下一个
            }
        }
    }
};

#endif //PARALLEL_AC_AUTOMATION_KMP_H
