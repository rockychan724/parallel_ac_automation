#include "KMP.h"
#include <cassert>

KMP::KMP(const std::string &pattern) {
    assert(!pattern.empty());
    this->pattern = pattern;
    this->next = new int[pattern.length()];
    KMP::GetNext();
}

KMP::~KMP() {
    delete this->next;
}

int KMP::Search(const std::string &s) {
    int n = s.length(), m = this->pattern.length();
    int i = 0, j = 0;
    while (i < n && j < m) {
        if (j == -1 || s[i] == this->pattern[j]) {
            i++, j++;
        } else {
            j = this->next[j];
        }
    }
    if (j == m) return (i - j);
    else return -1;
}

// @see: https://blog.csdn.net/v_july_v/article/details/7041827
void KMP::GetNext() {
    this->next[0] = -1;
    int len = this->pattern.length();
    int j = 0, k = -1;
    while (j < len - 1) {
        if (k == -1 || this->pattern[j] == this->pattern[k]) {
            next[j + 1] = k + 1;
            j++, k++;
        } else {
            k = next[k];
        }
    }
}

void KMP::GetNextOptimized() {

}

