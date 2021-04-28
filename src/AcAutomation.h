#ifndef PARALLEL_AC_AUTOMATION_ACAUTOMATION_H
#define PARALLEL_AC_AUTOMATION_ACAUTOMATION_H

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <map>

class AcAutomation {
public:
    explicit AcAutomation(bool=false);

    void Insert(std::string);

    void BuildFailPointer();

    std::vector<std::string> Search(std::string);

private:
    struct Node {
        int end_flag; // 记录从根节点到当前节点的路径是否构成一个单词
        std::map<char, std::shared_ptr<Node>> next; // 指向所有子节点的指针集
        std::shared_ptr<Node> fail; // 失配指针
        std::string path; // 记录从根节点到当前节点的路径构成的字符序列，以便匹配到单词后直接输出单词

        explicit Node() {
            end_flag = false;
            next.clear();
            fail = nullptr;
            path = "";
        }
     };
    std::shared_ptr<Node> root;
    bool case_sensitive_flag;
    bool update_fail_pointer;
};


#endif //PARALLEL_AC_AUTOMATION_ACAUTOMATION_H
