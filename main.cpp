#include "src/MatchBase.h"
#include "src/KMP.h"
#include "src/Trie.h"
#include "src/AcAutomation.h"
#include "src/ParallelAcAutomation.h"
#include "src/util.h"
#include "sys/time.h"

void DebugKMP() {
    std::cout << "\n\n======================= KMP =======================\n";
    std::vector<std::string> keywords = {"ABCDABD", "abab"};
    std::string text = "BBC ABCDAB ABCDABCDABDE";
    KMP<char> kmp;
    std::map<std::string, std::string> config = {{"use_optimize", "1"}};
    if (kmp.Init(config, keywords) == 0) {
        std::cout << "Init error!!!\n";
        return;
    }
    std::map<std::string, std::vector<int>> res = kmp.Search(text);
    std::cout << "text: \"" << text << "\"\n";
    std::cout << "match result: \n";
    for (const auto &kv: res) {
        std::cout << "\"" << kv.first << "\": ";
        std::for_each(kv.second.begin(), kv.second.end(), [](int pos) { std::cout << pos << " "; });
        std::cout << std::endl;
    }
    std::cout << "======================= KMP =======================\n";
}

void DebugTrie() {
    std::cout << "\n\n======================= Trie =======================\n";
    std::vector<std::string> keywords = {"in", "inn", "int", "tea", "ten", "to", "you", "ins"};
    std::vector<std::string> texts = {"you are a good man", "too young", "happy birthday to you", "te", "insert"};

    Trie<char> trie;
    std::map<std::string, std::string> config;
    if (trie.Init(config, keywords) == 0) {
        std::cout << "Init error!!!\n";
        return;
    }

    for (const auto &text: texts) {
        std::map<std::string, std::vector<int>> res = trie.Search(text);
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

void DebugAcAutomation() {
    std::cout << "\n\n======================= AC =======================\n";
    std::vector<std::string> keywords = {"She", "shr", "say", "her", "er", "e", "好的", "哈哈", "，小心", "小心", "心", "good", "千万别翻车了"};
    std::vector<std::string> texts = {"one day she say her has eaten many shrimps",
                                      "Very Good! 哈哈，好的，小心点，千万别再翻车了。"};

    AcAutomation<char> ac_tree;
    std::map<std::string, std::string> config = {{"case_sensitive", "0"}};
    if (ac_tree.Init(config, keywords) == 0) {
        std::cout << "Init error!!!\n";
        return;
    }

    for (const auto &text: texts) {
        std::map<std::string, std::vector<int>> res = ac_tree.Search(text);
        std::cout << "text: \"" << text << "\"\n";
        std::cout << "match result: \n";
        for (const auto &kv: res) {
            std::cout << "\"" << kv.first << "\": ";
            std::for_each(kv.second.begin(), kv.second.end(), [](int pos) { std::cout << pos << " "; });
            std::cout << std::endl;
        }
    }
    std::cout << "======================= AC =======================\n";
}

void DebugAcAutomationV2() {
    std::cout << "\n\n======================= 普通 AC自动机 =======================\n";
    std::vector<std::string> keywords = GetModeString("../data/words.txt");
    std::string text = GetText("../data/wiki_en_1.txt", false);
    std::cout << "keywords: ";
    std::for_each(keywords.begin(), keywords.end(), [](const std::string &a) { std::cout << "\"" << a << "\","; });
    std::cout << "\ncharacter num of text: " << text.length() << std::endl;

    AcAutomation<char> ac;
    std::map<std::string, std::string> config = {{"case_sensitive", "0"}};
    if (ac.Init(config, keywords) == 0) {
        std::cout << "Init error!!!\n";
        return;
    }

    struct timeval start{}, end{};
    gettimeofday(&start, nullptr);
    auto res = ac.Search(text);
    gettimeofday(&start, nullptr);
    double cost = (end.tv_sec * 1000.0 + end.tv_usec / 1000.0) - (start.tv_sec * 1000.0 + start.tv_usec / 1000.0);
    std::cout << "Run time " << cost << " ms\n";
    std::cout << "Match result: \n";
    for (const auto &kv: res) {
        std::cout << "\"" << kv.first << "\": ";
        std::for_each(kv.second.begin(), kv.second.end(), [](int pos) { std::cout << pos << " "; });
        std::cout << std::endl;
    }

    std::map<std::string, int> gt = GetEntryFrequency(text, keywords);
    std::map<std::string, int> pred = GetEntryFrequency<char>(res, keywords);
    bool is_right = JudgeCorrectness(gt, pred);
    std::cout << "Is right? " << is_right << std::endl;
    std::cout << "======================= 普通 AC自动机 =======================\n";
}

void DebugParallelAcAutomation() {
    std::cout << "\n\n======================= 并行 AC自动机 =======================\n";
    std::vector<std::string> keywords = GetModeString("../data/words.txt");
    std::string text = GetText("../data/wiki_en_1.txt", false);
    std::cout << "keywords: ";
    std::for_each(keywords.begin(), keywords.end(), [](const std::string &a) { std::cout << "\"" << a << "\","; });
    std::cout << "\ncharacter num of text: " << text.length() << std::endl;

    ParallelAcAutomation<char> pac;
    std::map<std::string, std::string> config = {{"case_sensitive", "0"},
                                                 {"p",              "7"}}; // p: segments of text
    if (pac.Init(config, keywords) == 0) {
        std::cout << "Init error!!!\n";
        return;
    }

    struct timeval start{}, end{};
    gettimeofday(&start, nullptr);
    auto res = pac.Search(text);
    gettimeofday(&start, nullptr);
    double cost = (end.tv_sec * 1000.0 + end.tv_usec / 1000.0) - (start.tv_sec * 1000.0 + start.tv_usec / 1000.0);
    std::cout << "Run time " << cost << " ms\n";
    std::cout << "Match result: \n";
    for (const auto &kv: res) {
        std::cout << "\"" << kv.first << "\": ";
        std::for_each(kv.second.begin(), kv.second.end(), [](int pos) { std::cout << pos << " "; });
        std::cout << std::endl;
    }

    std::map<std::string, int> gt = GetEntryFrequency(text, keywords);
    std::map<std::string, int> pred = GetEntryFrequency<char>(res, keywords);
    bool is_right = JudgeCorrectness(gt, pred);
    std::cout << "Is right? " << is_right << std::endl;
    std::cout << "======================= 并行 AC自动机 =======================\n";
}

void Debug() {
    DebugKMP();
    DebugTrie();
    DebugAcAutomation();
    DebugAcAutomationV2();
    DebugParallelAcAutomation();
}


void TestBase(const std::map<std::string, std::string> &config, const std::vector<std::string> &keywords, const std::string &text,
              const std::shared_ptr<MatchBase<char>> &matcher, const std::string &match_engine_type) {
    std::cout << "\n\n======================= " << match_engine_type << " =======================\n";
    // Init
    if (matcher->Init(config, keywords) == 0) {
        std::cout << "Init error!!!\n";
        return;
    }

    struct timeval start{}, end{};
    gettimeofday(&start, nullptr);
    // Search
    std::map<std::string, std::vector<int>> res = matcher->Search(text);
    gettimeofday(&start, nullptr);
    double cost = (end.tv_sec * 1000.0 + end.tv_usec / 1000.0) - (start.tv_sec * 1000.0 + start.tv_usec / 1000.0);

    // Output
    std::cout << "Run time " << cost << " ms\n";
    std::cout << "Text: \"" << text << "\"\n";
    std::cout << "Match result: \n";
    for (const auto &kv: res) {
        std::cout << "\"" << kv.first << "\": ";
        std::for_each(kv.second.begin(), kv.second.end(), [](int pos) { std::cout << pos << " "; });
        std::cout << std::endl;
    }
    std::cout << "======================= " << match_engine_type << " =======================\n";
}

void Test() {
    std::vector<std::string> keywords = GetModeString("../data/words.txt");
    std::string text = GetText("../data/wiki_en_1.txt");
    std::map<std::string, std::string> config;
    std::shared_ptr<MatchBase<char>> matcher;

    // KMP
    matcher = std::make_shared<KMP<char>>();
    config.clear();
    config.insert({"use_optimize", "1"});
    TestBase(config, keywords, text, matcher, "KMP");

    // Trie
    matcher = std::make_shared<Trie<char>>();
    TestBase(config, keywords, text, matcher, "Trie");

    // AC
    matcher = std::make_shared<AcAutomation<char>>();
    config.insert({"case_sensitive", "0"});
    TestBase(config, keywords, text, matcher, "AC");

    // AC
    matcher = std::make_shared<AcAutomation<char>>();
    config.insert({"p", "7"});
    TestBase(config, keywords, text, matcher, "PARA-AC");
}

int main() {
    Debug();
//    Test();
    return 0;
}
