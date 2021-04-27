#ifndef PARALLEL_AC_AUTOMATION_TRIE_H
#define PARALLEL_AC_AUTOMATION_TRIE_H

#include <map>

template<typename T>
class Trie {
public:
    Trie();

    ~Trie();

    void Insert(T *, int);

    bool Search(T *, int);

private:
    struct Node {
        T element;
        bool end_flag;
        std::map<T, Node *> next;

        Node() {
            element = nullptr;
            end_flag = false;
        }

        Node(T e, bool _end_flag) {
            element = e;
            end_flag = _end_flag;
        }
    };

    Node *root;
};

#endif //PARALLEL_AC_AUTOMATION_TRIE_H
