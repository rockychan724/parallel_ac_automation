#include "src/MatchBase.h"
#include "src/KMP.h"
#include "src/Trie.h"
#include "src/AcAutomation.h"
#include "src/ParallelAcAutomation.h"
#include "src/util.h"
#include "sys/time.h"

void TestKMP() {
    std::cout << "\n\n======================= KMP =======================\n";
    std::vector<std::string> keywords = {"ABCDABD", "abab"};
    std::string text = "BBC ABCDAB ABCDABCDABDE";
    std::shared_ptr<MatchBase<char>> kmp(new KMP<char>());
    std::map<std::string, std::string> config = {{"use_optimize", "1"}};
    if (kmp->Init(config, keywords) == 0) {
        std::cout << "Init error!!!\n";
        return;
    }
    std::map<std::string, std::vector<int>> res = kmp->Search(text);
    std::cout << "text: \"" << text << "\"\n";
    std::cout << "match result: \n";
    for (const auto &kv: res) {
        std::cout << "\"" << kv.first << "\": ";
        std::for_each(kv.second.begin(), kv.second.end(), [](int pos) { std::cout << pos << " "; });
        std::cout << std::endl;
    }
    std::cout << "======================= KMP =======================\n";
}

void TestTrie() {
    std::cout << "\n\n======================= Trie =======================\n";
    std::vector<std::string> keywords = {"in", "inn", "int", "tea", "ten", "to"};
    std::vector<std::string> texts = {"you are a good man", "too young", "happy birthday to you", "te", "insert"};

    std::shared_ptr<MatchBase<char>> trie(new Trie<char>());
    std::map<std::string, std::string> config;
    if (trie->Init(config, keywords) == 0) {
        std::cout << "Init error!!!\n";
        return;
    }

    for (const auto &text: texts) {
        std::map<std::string, std::vector<int>> res = trie->Search(text);
        std::cout << "text: \"" << text << "\"\n";
        std::cout << "match result: \n";
        for (const auto &kv: res) {
            std::cout << "\"" << kv.first << "\": ";
            std::for_each(kv.second.begin(), kv.second.end(), [](int pos) { std::cout << pos << " "; });
            std::cout << std::endl;
        }
    }
    std::cout << "======================= Trie =======================\n";
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

    struct timeval start{}, end{};
    gettimeofday(&start, nullptr);
    std::vector<std::string> res = ac_tree.Search(text);
    gettimeofday(&start, nullptr);
    double cost = (end.tv_sec * 1000.0 + end.tv_usec / 1000.0) - (start.tv_sec * 1000.0 + start.tv_usec / 1000.0);
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

    struct timeval start{}, end{};
    gettimeofday(&start, nullptr);
    std::vector<std::string> res = pac.Search(text);
    gettimeofday(&start, nullptr);
    double cost = (end.tv_sec * 1000.0 + end.tv_usec / 1000.0) - (start.tv_sec * 1000.0 + start.tv_usec / 1000.0);
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
    TestKMP();
    TestTrie();
//    TestAcAutomation();
//    TestAcAutomationV2();
//    TestParallelAcAutomation();
    return 0;
}
