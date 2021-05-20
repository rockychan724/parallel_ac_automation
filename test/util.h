#ifndef PARALLEL_AC_AUTOMATION_UTIL_H
#define PARALLEL_AC_AUTOMATION_UTIL_H

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

//#define DEBUG_COMPARE_GT_AND_PRED

std::vector<std::string> GetModeString(const std::string &file) {
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

std::string GetText(const std::string &file, bool ignore_spaces = false) {
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

std::map<std::string, int> GetEntryFrequencyFromString(const std::string &text, const std::vector<std::string> &words) {
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
std::map<std::string, int>
GetEntryFrequencyFromResult(const std::map<std::basic_string<CharType>, std::vector<int>> &results, const std::vector<std::string> &words) {
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

std::map<std::string, std::vector<int>> GetGT(std::string text, std::vector<std::string> words, bool case_sensitive) {
    if (!case_sensitive) {
        std::transform(text.begin(), text.end(), text.begin(), ::tolower);
    }
    std::map<std::string, std::vector<int>> res;
    for (auto &word : words) {
        if (!case_sensitive) {
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        }
        int pos = text.find(word);
        while (pos != -1) {
            if (res.find(word) == res.end()) {
                res.insert({word, {pos}});
            } else {
                res[word].push_back(pos);
            }
            pos = text.find(word, pos + 1);
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

bool JudgeCorrectness(const std::map<std::string, std::vector<int>> &gt, const std::map<std::string, std::vector<int>> &pred) {
#ifdef DEBUG_COMPARE_GT_AND_PRED
    std::cout << "****** Compare result with GT ******\n";
    for (const auto &kv: gt) {
        auto it = pred.find(kv.first);
        std::cout << "\"" << kv.first << "\": gt: {";
        std::for_each(kv.second.begin(), kv.second.end(), [](int pos){std::cout << pos << ",";});
        std::cout << "} actual value: {";
        if (it != pred.end())
            std::for_each(it->second.begin(), it->second.end(), [](int pos){std::cout << pos << ",";});
        std::cout << "}";
        std::cout << " is right? " << (kv.second == it->second) << std::endl;
    }
    std::cout << "****** Compare result with GT ******\n";
#endif
    return gt == pred;
}

#endif //PARALLEL_AC_AUTOMATION_UTIL_H
