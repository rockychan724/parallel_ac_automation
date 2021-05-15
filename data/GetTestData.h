#ifndef PARALLEL_AC_AUTOMATION_GETTESTDATA_H
#define PARALLEL_AC_AUTOMATION_GETTESTDATA_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

std::vector<std::string> GetModeString() {
    std::ifstream ifs("../data/words.txt");
    if (!ifs.is_open()) {
        std::cout << "Can't open file!!!" << std::endl;
        return std::vector<std::string>();
    } else {
        std::vector<std::string> model_strings;
        std::string line;
        while (getline(ifs, line)) {
            model_strings.push_back(line);
        }
        return model_strings;
    }
}

std::string GetText(bool ignore_spaces = true) {
    std::ifstream ifs("../data/wiki_en_1.txt");
    if (!ifs.is_open()) {
        std::cout << "Can't open file!!!" << std::endl;
        return "";
    } else {
        std::string text, line;
        while (getline(ifs, line)) {
            if (!ignore_spaces)
                std::remove_if(line.begin(), line.end(), [](const char &c) { return c == ' '; });
            text += line;
        }
        return text;
    }
}


#endif //PARALLEL_AC_AUTOMATION_GETTESTDATA_H
