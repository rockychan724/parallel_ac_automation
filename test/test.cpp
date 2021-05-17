#include "../src/MatchBase.h"
#include "../src/KMP.h"
#include "../src/Trie.h"
#include "../src/AcAutomation.h"
#include "../src/ParallelAcAutomation.h"
#include "util.h"
#include "sys/time.h"

void TestKMP() {
    std::cout << "\n\n======================= KMP =======================\n";
//    std::vector<std::string> keywords = {"ABCDABD", "abab"};
//    std::vector<std::string> texts = {"BBC ABCDAB ABCDABCDABDE"};
    std::vector<std::string> keywords = GetModeString("../data/words.txt");
    std::vector<std::string> texts = {GetText("../data/wiki_en_1.txt", false)};
    std::cout << "keywords: ";
    std::for_each(keywords.begin(), keywords.end(), [](const std::string &a) { std::cout << "\"" << a << "\","; });
    std::cout << std::endl;

    KMP<char> kmp;
    std::map<std::string, std::string> config = {{"case_sensitive", "1"},
                                                 {"use_optimize",   "1"}};
    if (kmp.Init(config, keywords) == 0) {
        std::cout << "Init error!!!\n";
        return;
    }

    struct timeval start{}, end{};
    for (const auto &text: texts) {
        gettimeofday(&start, nullptr);
        std::map<std::string, std::vector<int>> res = kmp.Search(text);
        gettimeofday(&end, nullptr);
        double cost = (end.tv_sec * 1000.0 + end.tv_usec / 1000.0) - (start.tv_sec * 1000.0 + start.tv_usec / 1000.0);
        std::cout << "======\n";
//        std::cout << "text: \"" << text << "\"\n";
        std::cout << "Run time " << cost << " ms\n";
        std::cout << "Match result: \n";
        for (const auto &kv: res) {
            std::cout << "\"" << kv.first << "\": ";
            std::for_each(kv.second.begin(), kv.second.end(), [](int pos) { std::cout << pos << " "; });
            std::cout << std::endl;
        }
        std::map<std::string, std::vector<int>> gt = GetGT(text, keywords, false);
        bool is_right = JudgeCorrectness(gt, res);
        std::cout << "Is right? " << is_right << std::endl;
    }
    std::cout << "======================= KMP =======================\n";
}

void TestTrie() {
    std::cout << "\n\n======================= Trie =======================\n";
//    std::vector<std::string> keywords = {"in", "inn", "int", "tea", "ten", "to", "you", "ins", "template", "plata"};
//    std::vector<std::string> texts = {"abc templata", "you are a good man", "too young", "happy biyouday to you", "te", "insert"};
    std::vector<std::string> keywords = GetModeString("../data/words.txt");
    std::vector<std::string> texts = {GetText("../data/wiki_en_1.txt", false)};
    std::cout << "keywords: ";
    std::for_each(keywords.begin(), keywords.end(), [](const std::string &a) { std::cout << "\"" << a << "\","; });
    std::cout << std::endl;

    Trie<char> trie;
    std::map<std::string, std::string> config = {{"case_sensitive", "1"}};
    if (trie.Init(config, keywords) == 0) {
        std::cout << "Init error!!!\n";
        return;
    }

    struct timeval start{}, end{};
    for (const auto &text: texts) {
        gettimeofday(&start, nullptr);
        std::map<std::string, std::vector<int>> res = trie.Search(text);
        gettimeofday(&end, nullptr);
        double cost = (end.tv_sec * 1000.0 + end.tv_usec / 1000.0) - (start.tv_sec * 1000.0 + start.tv_usec / 1000.0);
        std::cout << "======\n";
//        std::cout << "text: \"" << text << "\"\n";
        std::cout << "Run time " << cost << " ms\n";
        std::cout << "Match result: \n";
        for (const auto &kv: res) {
            std::cout << "\"" << kv.first << "\": ";
            std::for_each(kv.second.begin(), kv.second.end(), [](int pos) { std::cout << pos << " "; });
            std::cout << std::endl;
        }
        std::map<std::string, std::vector<int>> gt = GetGT(text, keywords, false);
        bool is_right = JudgeCorrectness(gt, res);
        std::cout << "Is right? " << is_right << std::endl;
    }
    std::cout << "======================= Trie =======================\n";
}

void TestAcAutomation() {
    std::cout << "\n\n======================= AC Automation =======================\n";
//    std::vector<std::string> keywords = {"She", "shr", "say", "her", "er", "e", "好的", "哈哈", "，小心", "小心", "心", "good", "千万别翻车了"};
//    std::vector<std::string> texts = {"one day she say her has eaten many shrimps", "Very Good! 哈哈，好的，小心点，千万别再翻车了。"};
    std::vector<std::string> keywords = GetModeString("../data/words.txt");
    std::vector<std::string> texts = {GetText("../data/wiki_en_1.txt", false)};
    std::cout << "keywords: ";
    std::for_each(keywords.begin(), keywords.end(), [](const std::string &a) { std::cout << "\"" << a << "\","; });
    std::cout << std::endl;

    AcAutomation<char> ac;
    std::map<std::string, std::string> config = {{"case_sensitive", "1"}};
    if (ac.Init(config, keywords) == 0) {
        std::cout << "Init error!!!\n";
        return;
    }

    struct timeval start{}, end{};
    for (const auto &text: texts) {
        gettimeofday(&start, nullptr);
        std::map<std::string, std::vector<int>> res = ac.Search(text);
        gettimeofday(&end, nullptr);
        double cost = (end.tv_sec * 1000.0 + end.tv_usec / 1000.0) - (start.tv_sec * 1000.0 + start.tv_usec / 1000.0);
        std::cout << "======\n";
//        std::cout << "text: \"" << text << "\"\n";
        std::cout << "Length of text: " << text.length() << std::endl;
        std::cout << "Run time " << cost << " ms\n";
        std::cout << "Match result: \n";
        for (const auto &kv: res) {
            std::cout << "\"" << kv.first << "\": ";
            std::for_each(kv.second.begin(), kv.second.end(), [](int pos) { std::cout << pos << " "; });
            std::cout << std::endl;
        }
        std::map<std::string, std::vector<int>> gt = GetGT(text, keywords, false);
        bool is_right = JudgeCorrectness(gt, res);
        std::cout << "Is right? " << is_right << std::endl;
    }
    std::cout << "======================= AC Automation =======================\n";
}

void TestParallelAcAutomation() {
    std::cout << "\n\n======================= Parallel AC Automation =======================\n";
    std::vector<std::string> keywords = GetModeString("../data/words.txt");
    std::vector<std::string> texts = {GetText("../data/wiki_en_1.txt", false)};
    std::cout << "keywords: ";
    std::for_each(keywords.begin(), keywords.end(), [](const std::string &a) { std::cout << "\"" << a << "\","; });
    std::cout << std::endl;

    ParallelAcAutomation<char> pac;
    std::map<std::string, std::string> config = {{"case_sensitive", "1"},
                                                 {"p",              "7"}}; // p: segments of text
    if (pac.Init(config, keywords) == 0) {
        std::cout << "Init error!!!\n";
        return;
    }

    struct timeval start{}, end{};
    for (const auto &text: texts) {
        gettimeofday(&start, nullptr);
        std::map<std::string, std::vector<int>> res = pac.Search(text);
        gettimeofday(&end, nullptr);
        double cost = (end.tv_sec * 1000.0 + end.tv_usec / 1000.0) - (start.tv_sec * 1000.0 + start.tv_usec / 1000.0);
        std::cout << "======\n";
//        std::cout << "text: \"" << text << "\"\n";
        std::cout << "Length of text: " << text.length() << std::endl;
        std::cout << "Run time " << cost << " ms\n";
        std::cout << "Match result: \n";
        for (const auto &kv: res) {
            std::cout << "\"" << kv.first << "\": ";
            std::for_each(kv.second.begin(), kv.second.end(), [](int pos) { std::cout << pos << " "; });
            std::cout << std::endl;
        }
        std::map<std::string, std::vector<int>> gt = GetGT(text, keywords, false);
        bool is_right = JudgeCorrectness(gt, res);
        std::cout << "Is right? " << is_right << std::endl;
    }
    std::cout << "======================= Parallel AC Automation =======================\n";
}

int main() {
    TestKMP();
    TestTrie();
    TestAcAutomation();
    TestParallelAcAutomation();
    return 0;
}