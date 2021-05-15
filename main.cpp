#include "src/KMP.h"
#include "src/Trie.h"
#include "src/AcAutomation.h"
#include "src/ParallelAcAutomation.h"
#include "data/GetTestData.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

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

void TestParallelAcAutomation() {
    std::vector<std::string> words = GetModeString();
    std::string text = GetText(false);
    int p = 5; // segments of text
    ParallelAcAutomation pac(words, p);
    std::vector<std::string> res = pac.Search(text);
    std::for_each(res.begin(), res.end(), [](const std::string &a) { std::cout << a << std::endl; });
}

int main() {
//    TestKMP();
//    TestTrie();
//    TestAcAutomation();
    TestParallelAcAutomation();
    return 0;
}
