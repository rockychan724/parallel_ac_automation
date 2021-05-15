#include "ParallelAcAutomation.h"
#include <mutex>
#include <algorithm>

std::mutex mut;

ParallelAcAutomation::ParallelAcAutomation(const std::vector<std::string> &keywords, int p) : p(p), que(p + 1) {
    this->BuildAcAutomation(keywords);
    auto longest_word = std::max_element(keywords.begin(), keywords.end(),
                                         [](const std::string &s1, const std::string &s2) { return s1.length() < s2.length(); });
    this->m = longest_word->length();
}


std::vector<std::string> ParallelAcAutomation::Search(const std::string& text) {
    std::vector<std::string> sub_texts;
    std::vector<std::string> boundary_texts;

    this->SplitText(text, sub_texts, boundary_texts);

    this->que.clear();
    this->result.clear();
    for (int i = 0; i < p; i++) {
        this->que[i].push(sub_texts[i]);
    }
    for (const auto & b_t: boundary_texts) {
        this->que[p].push(b_t);
    }

    for (int i = 0; i < p + 1; i++) {
        this->threads.emplace_back([this](int thread_id) { this->Run(thread_id); }, i);
    }

    for (int i = 0; i < p + 1; i++) {
        this->threads[i].join();
    }

    return this->result;
}

void ParallelAcAutomation::BuildAcAutomation(const std::vector<std::string> &keywords) {
    // Init a AC tree
    this->ac_tree = std::make_shared<AcAutomation>(false);
    // Insert keywords
    for (const auto &word: keywords) {
        this->ac_tree->Insert(word);
    }
    // Build fail pointers
    this->ac_tree->BuildFailPointer();
}

void ParallelAcAutomation::SplitText(const std::string &text, std::vector<std::string> &sub_texts,
                                     std::vector<std::string> &boundary_texts) const {
    int total_len = text.length();
    int sub_text_len = total_len / p;
    for (int i = 0; i < p; i++) {
        if (i < p - 1)
            sub_texts.push_back(text.substr(i * sub_text_len, sub_text_len));
        else
            sub_texts.push_back(text.substr(i * sub_text_len));
        if (i > 0) {
            boundary_texts.push_back(text.substr(std::max(0, i * sub_text_len - (m - 1)), (m - 1 * 2)));
        }
    }
}

void ParallelAcAutomation::Run(int thread_id) {
    while (true) {
        if (!this->que[thread_id].empty()) {
            std::string sub_text = this->que[thread_id].front();
            this->que[thread_id].pop();
            std::vector<std::string> sub_result = this->ac_tree->Search(sub_text);
            std::vector<std::string> dst;
            std::merge(this->result.begin(), this->result.end(), sub_result.begin(), sub_result.end(),
                       std::back_inserter(dst)); // merge sub_result to this->result;
            mut.lock();
            this->result = dst;
            mut.unlock();
            break;
        }
    }
}
