#include "src/MatchBase.h"
#include "src/KMP.h"
#include "src/Trie.h"
#include "src/AcAutomation.h"
#include "src/ParallelAcAutomation.h"
#include "test/util.h"
#include "sys/time.h"

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
    TestBase(config, keywords, text, matcher, "AC Automation");

    // AC
    matcher = std::make_shared<AcAutomation<char>>();
    config.insert({"p", "7"});
    TestBase(config, keywords, text, matcher, "Parallel AC Automation");
}

int main() {
    Test();
    return 0;
}
