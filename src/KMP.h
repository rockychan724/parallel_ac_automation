#ifndef PARALLEL_AC_AUTOMATION_KMP_H
#define PARALLEL_AC_AUTOMATION_KMP_H

#include "MatchBase.h"
#include <iostream>

template<typename T>
class KMP : public MatchBase<T> {
public:
    int Init(const std::map<std::string, std::string> &config, const std::vector<T> &_keywords);

    std::map<T, std::vector<int>> Search(const T &text);

private:
    std::vector<T> keywords;
    std::vector<std::vector<int>> nexts;

    bool CheckTType();

    static std::vector<int> GetNext(const T &pattern);

    static std::vector<int> GetNextOptimized(const T &pattern);
};

template<typename T>
int KMP<T>::Init(const std::map<std::string, std::string> &config, const std::vector<T> &_keywords) {
    if (!this->CheckTType()) {
        std::cout << "Type T is invaild." << std::endl;
        return 0;
    }

    // Check config parameter
    auto it = config.find("use_optimize");
    if (it == config.end()) {
        std::cout << "Parameter \"use_optimize\" is required in class KMP!!!\n";
        return 0;
    }
    int use_optimize = std::atoi(it->second.c_str());

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

// Now one keyword can only be found once.
template<typename T>
std::map<T, std::vector<int>> KMP<T>::Search(const T &s) {
    std::map<T, std::vector<int>> res;
    for (size_t t = 0; t < this->keywords.size(); t++) {
        T pattern = this->keywords[t];
        std::vector<int> next = this->nexts[t];
        int n = s.length(), m = pattern.length();
        int i = 0, j = 0;
        while (i < n && j < m) {
            if (j == -1 || s[i] == pattern[j]) {
                i++, j++;
            } else {
                j = next[j];
            }
        }
        if (j == m) {
            res.insert({pattern, {i - j}});
        }
    }
    return res;
}

template<typename T>
bool KMP<T>::CheckTType() {
    // .length()
    // [i]

    return true;
}

template<typename T>
std::vector<int> KMP<T>::GetNext(const T &pattern) {
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

template<typename T>
std::vector<int> KMP<T>::GetNextOptimized(const T &pattern) {
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


#endif //PARALLEL_AC_AUTOMATION_KMP_H
