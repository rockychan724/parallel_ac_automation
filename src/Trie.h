#ifndef PARALLEL_AC_AUTOMATION_TRIE_H
#define PARALLEL_AC_AUTOMATION_TRIE_H

#include <map>

//template<typename T>
#define T char
class Trie {
public:
    Trie(T, bool);

    ~Trie();

    void Insert(T *, int);

    bool Search(T *, int);

private:
    struct Node {
        T element;
        bool end_flag;
        std::map<T, Node *> next;

        Node(T e, bool _end_flag) {
            element = e;
            end_flag = _end_flag;
        }
    };

    Node *root;
};

#endif //PARALLEL_AC_AUTOMATION_TRIE_H
