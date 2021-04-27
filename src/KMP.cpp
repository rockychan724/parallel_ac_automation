#include "KMP.h"
#include <cassert>

KMP::KMP(const std::string &_pattern, bool use_optimize) {
    assert(!_pattern.empty());
    this->pattern = _pattern;
    this->next = new int[_pattern.length()];
    if (use_optimize)
        KMP::GetNextOptimized();
    else
        KMP::GetNext();
}

KMP::~KMP() {
    delete this->next;
}

void KMP::ResetPattern(const std::string &_pattern, bool use_optimize) {
    assert(!_pattern.empty());
    this->pattern = _pattern;
    delete this->next;
    this->next = new int[_pattern.length()];
    if (use_optimize)
        KMP::GetNextOptimized();
    else
        KMP::GetNext();
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

void KMP::GetNext() {
    this->next[0] = -1;
    int len = this->pattern.length();
    int j = 0, k = -1;
    while (j < len - 1) {
        if (k == -1 || this->pattern[j] == this->pattern[k]) {
            j++, k++;
            next[j] = k;
        } else {
            k = next[k];
        }
    }
}

void KMP::GetNextOptimized() {
    this->next[0] = -1;
    int len = this->pattern.length();
    int j = 0, k = -1;
    while (j < len - 1) {
        if (k == -1 || this->pattern[j] == this->pattern[k]) {
            j++, k++;
            if (this->pattern[j] != this->pattern[k])
                next[j] = k;
            else {
                next[j] = next[k];
            }
        } else {
            k = next[k];
        }
    }
}
