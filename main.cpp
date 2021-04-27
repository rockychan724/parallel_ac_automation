#include "src/KMP.h"
#include "src/Trie.h"
#include <iostream>
#include <string>
#include <vector>

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
    std::vector<std::string> strings = {"you are a good man", "happy birthday to you", "te", "insert"};
    Trie<char> tree;
    for (const auto &word: words) {
        tree.Insert(const_cast<char *>(word.c_str()), word.length());
    }
    for (const auto &s: strings) {
        std::cout << tree.Search(const_cast<char *>(s.c_str()), s.length()) << std::endl;
    }
}

int main() {
//    TestKMP();
    TestTrie();
    return 0;
}
