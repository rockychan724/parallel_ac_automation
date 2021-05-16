#ifndef PARALLEL_AC_AUTOMATION_TRIE_H
#define PARALLEL_AC_AUTOMATION_TRIE_H

#include "MatchBase.h"
#include <memory>
#include <algorithm>

template<typename CharType>
class Trie : public MatchBase<CharType> {
public:
    using MyString = std::basic_string<CharType>;

    virtual int Init(const std::map<std::string, std::string> &config, const std::vector<MyString> &keywords) {
        this->root = std::make_shared<TrieNode>();

        for (const auto &word: keywords) {
            this->Insert(word);
        }

        return 1;
    }

    virtual std::map<MyString, std::vector<int>> Search(const MyString &text) {
        std::map<MyString, std::vector<int>> res;
        std::shared_ptr<TrieNode> temp = this->root;
        for (int i = 0; i < text.length(); i++) {
            if (temp->end_flag) {
                if (res.find(temp->path) == res.end())
                    res.insert({temp->path, {i - (int)temp->path.length()}});
                else
                    res[temp->path].push_back(i - (int)temp->path.length());
            }
            if (temp->next.find(text[i]) == temp->next.end())
                temp = this->root;
            else
                temp = temp->next[text[i]];
        }
        return res;
    }

private:
    struct TrieNode {
        bool end_flag;
        std::map<CharType, std::shared_ptr<TrieNode>> next;
        MyString path;

        explicit TrieNode() {
            end_flag = false;
            next.clear();
            path = "";
        }
    };

    std::shared_ptr<TrieNode> root;

    // insert a word into ac tree
    void Insert(const MyString &word) {
        std::shared_ptr<TrieNode> temp = this->root;
        for (const auto &w : word) {
            if (temp->next.find(w) == temp->next.end()) {
                std::shared_ptr<TrieNode> new_node(new TrieNode());
                new_node->path = temp->path + w;
                temp->next.insert({w, new_node});
            }
            temp = temp->next[w];
        }
        temp->end_flag = true; // last char's flag of a word should be set true
    }
};

#endif //PARALLEL_AC_AUTOMATION_TRIE_H
