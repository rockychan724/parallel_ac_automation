#include "data/GetTestData.h"
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <random>
#include <functional>
#include <string>

void test1() {
    std::vector<std::string> words = GetModeString();
    std::string text = GetText(false);
    std::cout << "====== mode strings:" << std::endl;
    std::for_each(words.begin(), words.end(), [](const std::string &w) { std::cout << w << std::endl; });
    std::cout << "====== text:" << std::endl;
    std::cout << text << std::endl;
}


void test2()
{
    // 以随机数填充 vector
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<> dis(0, 9);

    std::vector<int> v1(10), v2(10);
    std::generate(v1.begin(), v1.end(), std::bind(dis, std::ref(mt)));
    std::generate(v2.begin(), v2.end(), std::bind(dis, std::ref(mt)));

    // 排序
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    // 输出 v1
    std::cout << "v1 : ";
    std::copy(v1.begin(), v1.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';

    // 输出 v2
    std::cout << "v2 : ";
    std::copy(v2.begin(), v2.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';

    // 归并
    std::vector<int> dst;
    std::merge(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(dst));

    // 输出
    std::cout << "dst: ";
    std::copy(dst.begin(), dst.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';
}

void test3() {
    std::string a("abcdefg");
    std::cout << a.substr(2, 20) << std::endl;
}

int main(){
    test3();
    return 0;
}