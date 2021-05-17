#ifndef PARALLEL_AC_AUTOMATION_ACAUTOMATION_H
#define PARALLEL_AC_AUTOMATION_ACAUTOMATION_H

#include "MatchBase.h"
#include <iostream>
#include <queue>

template<typename CharType>
class AcAutomation : public MatchBase<CharType> {
public:
    using MyString = std::basic_string<CharType>;

    virtual int Init(const std::map<std::string, std::string> &config, const std::vector<MyString> &keywords) {
        return this->AcInit(config, keywords);
    }

    virtual std::map<MyString, std::vector<int>> Search(const MyString &text) {
        return this->AcSearch(text);
    }


protected:
    int AcInit(const std::map<std::string, std::string> &config, std::vector<MyString> keywords) {
        // Check config parameter
        auto it = config.find("case_sensitive");
        if (it == config.end()) {
            std::cout << "Parameter \"case_sensitive\" is required in class AcAutomation!!!\n";
            return 0;
        }
        this->case_sensitive = std::stoi(it->second);

        this->root = std::make_shared<AcNode>();

        // Insert keywords
        for (auto &word: keywords) {
            if (!this->case_sensitive)
                std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            this->Insert(word);
        }

        // Build fail pointers
        this->BuildFailPointer();
        return 1;
    }

    std::map<MyString, std::vector<int>> AcSearch(MyString text, int offset = 0) {
        if (!this->case_sensitive)
            std::transform(text.begin(), text.end(), text.begin(), ::tolower);

        std::map<MyString, std::vector<int>> res;
        std::shared_ptr<AcNode> p = this->root;
        for (int i = 0; i < text.length(); i++) {
            // p==nullptr 说明当前指针p为root节点的失配指针，因为只有root节点的是失配指针为nullptr
            while (p && p->next.find(text[i]) == p->next.end())
                p = p->fail;
            if (!p)
                p = this->root;
            else
                p = p->next[text[i]];

            // 找到当前所有匹配的单词
            std::shared_ptr<AcNode> temp = p;
            while (temp != this->root) {
                if (temp->end_flag) {
                    if (res.find(temp->path) == res.end())
                        res.insert({temp->path, {i - ((int) temp->path.length() - 1) + offset}});
                    else
                        res[temp->path].push_back(i - ((int) temp->path.length() - 1) + offset);
                }
                temp = temp->fail;
            }
        }
        return res;
    }

private:
    struct AcNode {
        int end_flag; // 记录从根节点到当前节点的路径是否构成一个单词
        std::map<char, std::shared_ptr<AcNode>> next; // 指向所有子节点的指针集
        std::shared_ptr<AcNode> fail; // 失配指针
        std::string path; // 记录从根节点到当前节点的路径构成的字符序列，以便匹配到单词后直接输出单词

        explicit AcNode() {
            end_flag = false;
            next.clear();
            fail = nullptr;
            path = "";
        }
    };

    std::shared_ptr<AcNode> root;
    bool case_sensitive;

    // insert a word into ac tree
    void Insert(const MyString &word) {
        std::shared_ptr<AcNode> temp = this->root;
        for (const auto &w : word) {
            if (temp->next.find(w) == temp->next.end()) {
                std::shared_ptr<AcNode> new_node(new AcNode());
                new_node->path = temp->path + w;
                temp->next.insert({w, new_node});
            }
            temp = temp->next[w];
        }
        temp->end_flag = true; // last char's flag of a word should be set true
    }

    // use BFS algorithm to build all fail pointers in this tree
    void BuildFailPointer() {
        std::queue<std::shared_ptr<AcNode>> q;
        q.push(this->root); // 根节点失配指针为 nullptr
        while (!q.empty()) {
            std::shared_ptr<AcNode> current_node = q.front(); // 获取队头结点
            q.pop();
            for (const auto &iter: current_node->next) { // 遍历当前节点的所有子节点
                char w = iter.first;
                std::shared_ptr<AcNode> son_node = iter.second;
                // 暂时发现这个操作是多余的，可以注释
                // son_node->fail = nullptr; // AcAutomation::BuildFailPointer() 被多次调用时，在更新 fail 指针之前需要将 fail 指针归零
                if (current_node == this->root) {
                    son_node->fail = this->root;
                } else {
                    std::shared_ptr<AcNode> p = current_node->fail;
                    while (p) {
                        if (p->next.find(w) != p->next.end()) {
                            son_node->fail = p->next[w];
                            break;
                        }
                        p = p->fail;
                    }
                    if (!p)
                        son_node->fail = this->root;
                }
                q.push(son_node); // 将当前节点的各个子节点加入队列
            }
        }
    }
};


#endif //PARALLEL_AC_AUTOMATION_ACAUTOMATION_H
