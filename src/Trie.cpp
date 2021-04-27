#include "Trie.h"

//template<typename T>
Trie::Trie(T e, bool _end_flag) {
    this->root = new Node(e, _end_flag);
}

//template<typename T>
Trie::~Trie() = default;

//template<typename T>
void Trie::Insert(T *word, int word_length) {
    Node *temp = this->root;
    for (int i = 0; i < word_length; i++) {
        if (temp->next.find(word[i]) == temp->next.end()) {
            Node *new_node;
            if (i == word_length - 1)
                new_node = new Node(word[i], true);
            else
                new_node = new Node(word[i], false);
            temp->next.insert({word[i], new_node});
        }
        temp = temp->next[word[i]];
    }
}

//template<typename T>
bool Trie::Search(T *s, int s_length) {
    Node *temp = this->root;
    for (int i = 0; i < s_length; i++) {
        if (temp->end_flag)
            return true;
        if (temp->next.find(s[i]) != temp->next.end())
            temp = temp->next[s[i]];
    }
    return false;
}



