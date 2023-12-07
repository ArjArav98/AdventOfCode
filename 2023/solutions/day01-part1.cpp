/* Copyright 2023 Arjun Aravind */

#include<cassert>
#include<algorithm>
#include<exception>
#include<iostream>
#include<locale>
#include<fstream>
#include<string>
#include<utility>

int get_calibration_value_for_line(std::string line) {
    int first = 0, second = 0, line_length = line.length();
    int first_index = -1;
    bool first_not_set = true, second_not_set = true;

    for (
        int index = 0, back_index = line_length-(index+1);
        index < line_length;
        index++, back_index--
    ) {
        char front_char = line.at(index), back_char = line.at(back_index);

        if (first_not_set && std::isdigit(front_char)) {
            bool indexes_are_same = index == (back_index);
            first = front_char - '0';
            first_index = index;
            first_not_set = false;
        }

        if (second_not_set && std::isdigit(back_char)) {
            second = back_char - '0';
            second_not_set = false;
        }

        // Optimisations
        if (!first_not_set && !second_not_set) break;
        if (back_index < first_index) break;
    }

    return first*10 + second;
}

int main() {
    /* Tests */
    typedef struct { std::string input; int expected; } testcase;
    std::vector<testcase> testcases {
        {"nine7ckxh", 77},
        {"fourjhqxgjrms9oneone78four1", 91},
        {"1abc2", 12},
        {"pqr3stu8vwx", 38},
        {"a1b2c3d4e5f", 15},
        {"treb7uchet", 77},
        {"trebuchet", 0},
        {"dpvcxz21seven", 21}
    };

    std::for_each(testcases.begin(), testcases.end(), [] (testcase tcase) {
        int output = get_calibration_value_for_line(tcase.input);
        std::cout <<
            "Testcase " + tcase.input + ": Is " + std::to_string(output) +
            ", Should be " + std::to_string(tcase.expected) + "\n";
        assert(output == tcase.expected);
    });

    /*---*/

    std::fstream file("01A-input.txt", std::fstream::in);
    std::string line;
    int sum = 0;

    do {
        std::getline(file, line);
        sum += get_calibration_value_for_line(line);
    } while (!file.eof());

    std::cout << sum << std::endl;

    file.close();
    return 0;
}
