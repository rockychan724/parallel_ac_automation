#ifndef PARALLEL_AC_AUTOMATION_PARALLELACAUTOMATION_H
#define PARALLEL_AC_AUTOMATION_PARALLELACAUTOMATION_H

#include "AcAutomation.h"
#include <string>
#include <vector>
#include <queue>
#include <memory>
#include <thread>


class ParallelAcAutomation {
public:
    ParallelAcAutomation(const std::vector<std::string> &, unsigned int);

    std::vector<std::string> Search(const std::string &);

private:
    std::shared_ptr<AcAutomation> ac_tree;
    std::vector<std::thread> threads;
    std::vector<std::queue<std::string>> que; // storage of public resources
    std::vector<std::string> result;
    int m; // length of longest keyword
    int p; // segments num of text

    void BuildAcAutomation(const std::vector<std::string> &);

    void SplitText(const std::string &, std::vector<std::string> &, std::vector<std::string> &) const;

    void Run(int);
};


#endif //PARALLEL_AC_AUTOMATION_PARALLELACAUTOMATION_H
