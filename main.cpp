#include "src/KMP.h"
#include "src/Trie.h"
#include "src/AcAutomation.h"
#include "src/ParallelAcAutomation.h"
#include "src/util.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "sys/timeb.h"

void TestKMP() {
    std::string s = "BBC ABCDAB ABCDABCDABDE", pattern = "ABCDABD";
    KMP kmp(pattern);
    std::cout << kmp.Search(s) << std::endl;
    s = "abacababc", pattern = "abab";
    kmp.ResetPattern(pattern);
    std::cout << kmp.Search(s) << std::endl;
}

void TestTrie() {
    std::vector<std::string> words = {"in", "inn", "int", "tea", "ten", "to"};
    std::vector<std::string> strings = {"you are a good man", "too young", "happy birthday to you", "te", "insert"};
    Trie tree('\0', false);
    for (const auto &word: words) {
        tree.Insert(const_cast<char *>(word.c_str()), word.length());
    }
    for (const auto &s: strings) {
        std::cout << tree.Search(const_cast<char *>(s.c_str()), s.length()) << std::endl;
    }
}

void TestAcAutomation() {
    std::vector<std::string> words = {"she", "shr", "say", "her", "er", "e"};
    std::vector<std::string> strings = {"one day she say her has eaten many shrimps",
                                        "Very Good! 哈哈，好的，小心点，千万别再翻车了。"};

    AcAutomation ac_tree;

    for (const auto &word: words) {
        ac_tree.Insert(word);
    }
    ac_tree.BuildFailPointer();
    for (const auto &s: strings) {
        std::vector<std::string> res = ac_tree.Search(s);
        std::cout << s << std::endl;
        std::for_each(res.begin(), res.end(), [](const std::string &a) { std::cout << a << std::endl; });
    }

    words = {"好的", "哈哈", "，小心", "小心", "心", "good", "千万别翻车了"};
    for (const auto &word: words) {
        ac_tree.Insert(word);
    }
    ac_tree.BuildFailPointer();
    for (const auto &s: strings) {
        std::vector<std::string> res = ac_tree.Search(s);
        std::cout << s << std::endl;
        std::for_each(res.begin(), res.end(), [](const std::string &a) { std::cout << a << std::endl; });
    }
}

void TestAcAutomationV2() {
    std::cout << "\n\n======================= 普通 AC自动机 =======================\n";
    std::vector<std::string> words = GetModeString("../data/words.txt");
    std::string text = GetText("../data/wiki_en_1.txt", false);
    std::cout << "keywords: ";
    std::for_each(words.begin(), words.end(), [](const std::string &a) { std::cout << "\"" << a << "\","; });
    std::cout << "\ncharacter num of text: " << text.length() << std::endl;

    AcAutomation ac_tree;
    // Insert keywords
    for (const auto &word: words) {
        ac_tree.Insert(word);
    }
    // Build fail pointers
    ac_tree.BuildFailPointer();

    struct timeb start{}, end{};
    ftime(&start);
    std::vector<std::string> res = ac_tree.Search(text);
    ftime(&end);
    long cost = (end.time * 1000 + end.millitm) - (start.time * 1000 + start.millitm);
    std::cout << "Run time " << cost << " ms\n";
    std::cout << "Match result: ";
    std::for_each(res.begin(), res.end(), [](const std::string &a) { std::cout << "\"" << a << "\","; });
    std::cout << std::endl;

    std::map<std::string, int> gt = GetEntryFrequency(text, words);
    std::map<std::string, int> pred = GetEntryFrequency(res, words);
    bool is_right = JudgeCorrectness(gt, pred);
    std::cout << "Is right? " << is_right << std::endl;
    std::cout << "======================= 普通 AC自动机 =======================\n";
}

void TestParallelAcAutomation() {
    std::cout << "\n\n======================= 并行 AC自动机 =======================\n";
    std::vector<std::string> words = GetModeString("../data/words.txt");
    std::string text = GetText("../data/wiki_en_1.txt", false);
    std::cout << "keywords: ";
    std::for_each(words.begin(), words.end(), [](const std::string &a) { std::cout << "\"" << a << "\","; });
    std::cout << "\ncharacter num of text: " << text.length() << std::endl;

    unsigned int p = 7; // segments of text
    ParallelAcAutomation pac(words, p);

    struct timeb start{}, end{};
    ftime(&start);
    std::vector<std::string> res = pac.Search(text);
    ftime(&end);
    long cost = (end.time * 1000 + end.millitm) - (start.time * 1000 + start.millitm);
    std::cout << "Run time " << cost << " ms\n";
    std::cout << "Match result: ";
    std::for_each(res.begin(), res.end(), [](const std::string &a) { std::cout << "\"" << a << "\","; });
    std::cout << std::endl;

    std::map<std::string, int> gt = GetEntryFrequency(text, words);
    std::map<std::string, int> pred = GetEntryFrequency(res, words);
    bool is_right = JudgeCorrectness(gt, pred);
    std::cout << "Is right? " << is_right << std::endl;
    std::cout << "======================= 并行 AC自动机 =======================\n";
}

int main() {
//    TestKMP();
//    TestTrie();
//    TestAcAutomation();
//    TestAcAutomationV2();
    TestParallelAcAutomation();
    return 0;
}
