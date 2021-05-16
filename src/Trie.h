#ifndef PARALLEL_AC_AUTOMATION_TRIE_H
#define PARALLEL_AC_AUTOMATION_TRIE_H

#include "MatchBase.h"
#include <memory>

template<typename CharType>
class Trie : public MatchBase<CharType> {
public:
    using MyString = std::basic_string<CharType>;

    int Init(const std::map<std::string, std::string> &config, const std::vector<MyString> &keywords) {
        this->root = std::make_shared<Node>();

        for (const auto &word: keywords) {
            this->Insert(word);
        }

        return 1;
    }

    std::map<MyString, std::vector<int>> Search(const MyString &text) {
        std::map<MyString, std::vector<int>> res;
        std::shared_ptr<Node> temp = this->root;
        for (int i = 0; i < text.length(); i++) {
            if (temp->end_flag)
                res.insert({temp->path, {i - (int)temp->path.length()}});
            if (temp->next.find(text[i]) == temp->next.end())
                temp = this->root;
            else
                temp = temp->next[text[i]];
        }
        return res;
    }

protected:
    struct Node {
        bool end_flag;
        std::map<CharType, std::shared_ptr<Node>> next;
        MyString path;

        explicit Node() {
            end_flag = false;
            next.clear();
            path = "";
        }
    };

    std::shared_ptr<Node> root;

    void Insert(const MyString &word) {
        std::shared_ptr<Node> temp = this->root;
        for (const auto &w: word) {
            if (temp->next.find(w) == temp->next.end()) {
                std::shared_ptr<Node> new_node(new Node());
                new_node->path = temp->path + w;
                temp->next.insert({w, new_node});
            }
            temp = temp->next[w];
        }
        temp->end_flag = true;
    }
};

#endif //PARALLEL_AC_AUTOMATION_TRIE_H
