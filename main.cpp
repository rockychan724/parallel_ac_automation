#include "src/KMP.h"
#include <iostream>
#include <string>

void TestKMP() {
    std::string s = "BBC ABCDAB ABCDABCDABDE", pattern = "ABCDABD";
    KMP kmp(pattern);
    std::cout << kmp.Search(s) << std::endl;
}

int main() {
    TestKMP();
    return 0;
}
