#ifndef PARALLEL_AC_AUTOMATION_PARALLELACAUTOMATION_H
#define PARALLEL_AC_AUTOMATION_PARALLELACAUTOMATION_H

#include "AcAutomation.h"
#include <thread>
#include <mutex>

std::mutex mut;

template<typename CharType>
class ParallelAcAutomation : public AcAutomation<CharType> {
public:
    using MyString = std::basic_string<CharType>;

    int Init(const std::map<std::string, std::string> &config, const std::vector<MyString> &keywords) {
        if (this->_CheckConfig(config) == 0)
            return 0;

        this->que.resize(p + 1);
        auto longest_word = std::max_element(keywords.begin(), keywords.end(),
                                             [](const std::string &s1, const std::string &s2) { return s1.length() < s2.length(); });
        this->m = longest_word->length();

        return this->AcInit(config, keywords);
    }

    std::map<MyString, std::vector<int>> Search(const MyString &text) {
        std::vector<std::tuple<std::string, int>> sub_texts;
        std::vector<std::tuple<std::string, int>> boundary_texts;

        this->SplitText(text, sub_texts, boundary_texts);


        // Push data into queue
        for (int i = 0; i < p; i++) {
            this->que[i].push(sub_texts[i]);
        }
        for (const auto &b_t: boundary_texts) {
            this->que[p].push(b_t);
        }

        this->result.clear();
        // Launch all threads
        for (int i = 0; i < p + 1; i++) {
            this->threads.emplace_back([this](int thread_id) { this->Run(thread_id); }, i);
        }

        // Wait for all threads to end
        for (int i = 0; i < p + 1; i++) {
            this->threads[i].join();
        }

        // Sort every keyword's all positions in this->result.
        for (auto &kv: this->result) {
            std::sort(kv.second.begin(), kv.second.end());
        }

        return this->result;
    }

private:
    std::vector<std::thread> threads;
    std::vector<std::queue<std::tuple<std::string, int>>> que; // Each queue stores segments of the text and its offset position.
    std::map<MyString, std::vector<int>> result;
    int m; // length of longest keyword
    int p; // segments num of text

    int _CheckConfig(const std::map<std::string, std::string> &config) {
        // Check config parameter
        auto it = config.find("p");
        if (it == config.end()) {
            std::cout << "Parameter \"p\" is required in class ParallelAcAutomation!!!\n";
            return 0;
        }
        this->p = std::stoi(it->second);
        if (this->p < 1) {
            std::cout << "Parameter \"p\" should be greater than 0 in class ParallelAcAutomation!!!\n";
            return 0;
        }
        return 1;
    }

    void SplitText(const std::string &text, std::vector<std::tuple<std::string, int>> &sub_texts,
                   std::vector<std::tuple<std::string, int>> &boundary_texts) {
        int total_len = text.length();
        int sub_text_len = total_len / this->p;
        for (int i = 0; i < this->p; i++) {
            if (i < (this->p - 1))
                sub_texts.push_back({text.substr(i * sub_text_len, sub_text_len), i * sub_text_len});
            else
                sub_texts.push_back({text.substr(i * sub_text_len), i * sub_text_len});
            if (i > 0) {
                int start_position = std::max(0, i * sub_text_len - (this->m - 1));
                boundary_texts.push_back({text.substr(start_position, (this->m - 1) * 2), start_position});
            }
        }
    }

    void Run(int thread_id) {
        if (!this->que[thread_id].empty()) {
            std::string sub_text;
            int offset;
            std::tie(sub_text, offset) = this->que[thread_id].front();
            this->que[thread_id].pop();
            std::map<MyString, std::vector<int>> sub_result = this->AcSearch(sub_text, offset);
            mut.lock();
            // merge sub_result to this->result;
            this->MergeResult(sub_result);
            mut.unlock();
        }
    }

    void MergeResult(std::map<MyString, std::vector<int>> sub_result) {
        for (const auto &kv : sub_result) {
            auto it = this->result.find(kv.first);
            if (it == this->result.end()) {
                this->result.insert(kv);
            } else {
                std::vector<int> dst;
                std::merge(this->result[kv.first].begin(), this->result[kv.first].end(), kv.second.begin(), kv.second.end(),
                           std::back_inserter(dst));
                this->result[kv.first] = dst;
            }
        }
    }
};


#endif //PARALLEL_AC_AUTOMATION_PARALLELACAUTOMATION_H
