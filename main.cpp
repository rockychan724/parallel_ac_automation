#include "src/MatchBase.h"
#include "src/KMP.h"
#include "src/Trie.h"
#include "src/AcAutomation.h"
#include "src/ParallelAcAutomation.h"
#include "test/util.h"
#include "sys/time.h"

//#define PRINT_MATCH_RESULT

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
    gettimeofday(&end, nullptr);
    double cost = (end.tv_sec * 1000.0 + end.tv_usec / 1000.0) - (start.tv_sec * 1000.0 + start.tv_usec / 1000.0);

    // Output
    std::cout << "Length of text: " << text.length() << std::endl;
    std::cout << "Run time " << cost << " ms\n";
#ifdef PRINT_MATCH_RESULT
    std::cout << "Match result: \n";
    for (const auto &kv: res) {
        std::cout << "\"" << kv.first << "\": ";
        std::for_each(kv.second.begin(), kv.second.end(), [](int pos) { std::cout << pos << " "; });
        std::cout << std::endl;
    }
#endif
    std::map<std::string, std::vector<int>> gt = GetGT(text, keywords, false);
    bool is_right = JudgeCorrectness(gt, res);
    std::cout << "Is right? " << is_right << std::endl;
    std::cout << "======================= " << match_engine_type << " =======================\n";
}

void Test() {
    std::vector<std::string> keywords = GetModeString("../data/words.txt");
    std::string text = GetText("../data/wiki_en_100.txt");
    std::map<std::string, std::string> config;
    std::shared_ptr<MatchBase<char>> matcher;
//    std::cout << "text: \"" << text << "\"\n";
    std::cout << "keywords: ";
    std::for_each(keywords.begin(), keywords.end(), [](const std::string &a) { std::cout << "\"" << a << "\","; });
    std::cout << std::endl;

    // KMP
    matcher = std::make_shared<KMP<char>>();
    config.insert({"case_sensitive", "1"});
    config.insert({"use_optimize", "1"});
    TestBase(config, keywords, text, matcher, "KMP");

    // Trie
    matcher = std::make_shared<Trie<char>>();
    TestBase(config, keywords, text, matcher, "Trie");

    // AC Automation
    matcher = std::make_shared<AcAutomation<char>>();
    TestBase(config, keywords, text, matcher, "AC Automation");

    // Parallel AC Automation
    matcher = std::make_shared<ParallelAcAutomation<char>>();
    config.insert({"p", "7"});
    TestBase(config, keywords, text, matcher, "Parallel AC Automation");
}

int main() {
    Test();
    return 0;
}
