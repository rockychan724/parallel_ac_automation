#ifndef PARALLEL_AC_AUTOMATION_UTIL_H
#define PARALLEL_AC_AUTOMATION_UTIL_H

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

std::vector<std::string> GetModeString(const std::string& file) {
    std::ifstream ifs(file.c_str());
    if (!ifs.is_open()) {
        std::cout << "Can't open file!!!" << std::endl;
        return std::vector<std::string>();
    } else {
        std::vector<std::string> model_strings;
        std::string line;
        while (getline(ifs, line)) {
            model_strings.push_back(line);
        }
        return model_strings;
    }
}

std::string GetText(const std::string& file, bool ignore_spaces = false) {
    std::ifstream ifs(file.c_str());
    if (!ifs.is_open()) {
        std::cout << "Can't open file!!!" << std::endl;
        return "";
    } else {
        std::string text, line;
        while (getline(ifs, line)) {
            if (ignore_spaces)
                std::remove_if(line.begin(), line.end(), [](const char &c) { return c == ' '; });
            text += line;
        }
        return text;
    }
}

std::map<std::string, int> GetEntryFrequency(const std::string &text, const std::vector<std::string> &words) {
    std::map<std::string, int> res;
    for (auto &word : words) {
        res.insert({word, 0});
        size_t pos = text.find(word);
        while (pos != std::string::npos) {
            res[word]++;
            pos = text.find(word, pos + 1);
        }
    }
    return res;
}

template<class CharType>
std::map<std::string, int> GetEntryFrequency(const std::map<std::basic_string<CharType>, std::vector<int>> &results, const std::vector<std::string> &words) {
    std::map<std::string, int> res;
    for (auto &word : words) {
        auto it = results.find(word);
        if (it == results.end()) {
            res.insert({word, 0});
        } else {
            res.insert({word, it->second.size()});
        }
    }
    return res;
}

bool JudgeCorrectness(const std::map<std::string, int> &gt, const std::map<std::string, int> &pred) {
    std::cout << "keyword correct_frequency actual_frequency\n";
    for (const auto &kv: gt) {
        std::cout << "\"" << kv.first << "\": " << kv.second;
        auto it = pred.find(kv.first);
        if (it != pred.end())
            std::cout << " " << it->second;
        std::cout << std::endl;
    }
    return gt == pred;
}

#endif //PARALLEL_AC_AUTOMATION_UTIL_H
