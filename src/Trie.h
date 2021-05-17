#ifndef PARALLEL_AC_AUTOMATION_TRIE_H
#define PARALLEL_AC_AUTOMATION_TRIE_H

#include "MatchBase.h"
#include <memory>
#include <algorithm>

template<typename CharType>
class Trie : public MatchBase<CharType> {
public:
    using MyString = std::basic_string<CharType>;

    int Init(const std::map<std::string, std::string> &config, const std::vector<MyString> &keywords) {
        if (this->CheckConfig(config) == 0)
            return 0;

        this->root = std::make_shared<TrieNode>();
        for (auto word: keywords) {
            if (!this->case_sensitive)
                std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            this->Insert(word);
        }

        return 1;
    }

    std::map<MyString, std::vector<int>> Search(const MyString &_text) {
        MyString text = _text;
        if (!this->case_sensitive)
            std::transform(text.begin(), text.end(), text.begin(), ::tolower);
        std::map<MyString, std::vector<int>> res;
        std::shared_ptr<TrieNode> temp = this->root;
        for (int i = 0; i < text.length(); i++) {
            if (temp->next.find(text[i]) != temp->next.end()) // 若当前节点的子节点能匹配text[i]，则更新当前节点为其子节点
                temp = temp->next[text[i]];
            else if (temp != this->root) { // 若当前节点的子节点不能匹配text[i] 且 当前节点不是根节点，则从头开始匹配，因此更新当前节点为 根节点 或 根节点的子节点
                // 更新的时候注意：当前节点前面的所有节点（即从根节点到当前节点的所有节点，不包括根节点和当前节点）都是匹配过的。因此，更新的时候考虑要回退，
                // 前面匹配过的部分需要重新匹配。具体做法是从匹配过的串中的第二位字符开始重新从节点匹配。
                i = i - (temp->path.length() - 1);
                if (this->root->next.find(text[i]) != this->root->next.end())
                    temp = this->root->next[text[i]];
                else // Search from root node
                    temp = this->root;
            }

            if (temp->end_flag) {
                if (res.find(temp->path) == res.end())
                    res.insert({temp->path, {i - ((int) temp->path.length() - 1)}});
                else
                    res[temp->path].push_back(i - ((int) temp->path.length() - 1));
            }
        }
        return res;
    }

protected:
    int CheckConfig(const std::map<std::string, std::string> &config) {
        // Check config parameter
        auto it = config.find("case_sensitive");
        if (it == config.end()) {
            std::cout << "Parameter \"case_sensitive\" is required in class Trie!!!\n";
            return 0;
        }
        this->case_sensitive = std::stoi(it->second);
        return 1;
    }

private:
    struct TrieNode {
        bool end_flag;
        std::map<CharType, std::shared_ptr<TrieNode>> next;
        MyString path;

        explicit TrieNode() {
            end_flag = false;
            next.clear();
            path = "";  // TODO: to be removed
        }
    };

    std::shared_ptr<TrieNode> root;
    bool case_sensitive;

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
