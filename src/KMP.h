#ifndef PARALLEL_AC_AUTOMATION_KMP_H
#define PARALLEL_AC_AUTOMATION_KMP_H

#include <string>

class KMP {
public:
    explicit KMP(const std::string &, bool=true);

    ~KMP();

    void ResetPattern(const std::string &, bool=true);

    int Search(const std::string &);

private:
    std::string pattern;
    int *next;

    void GetNext();

    void GetNextOptimized();
};


#endif //PARALLEL_AC_AUTOMATION_KMP_H
