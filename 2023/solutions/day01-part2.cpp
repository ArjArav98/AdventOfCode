/* Copyright 2023 Arjun Aravind */

#include<cassert>
#include<algorithm>
#include<exception>
#include<iostream>
#include<map>
#include<locale>
#include<fstream>
#include<string>
#include<utility>
#include<regex>

struct NumStringMatch { int index; int value; };

int get_value_of_num_string_match(std::string match) {
    std::map<std::string, int> num_strings {
        {"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5},
        {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9},
    };

    return num_strings.at(match);
}

NumStringMatch get_first_num_string_match(std::string line) {
    std::regex number_regex(
        "(?=(one|two|three|four|five|six|seven|eight|nine)).");
    auto matches = std::sregex_iterator(line.begin(), line.end(), number_regex);
    auto end_sequence_iterator = std::sregex_iterator();

    if (matches == end_sequence_iterator)
        return { static_cast<int>(line.length()), 0 };  // no matches

    return {
        static_cast<int>(matches->position()),
        get_value_of_num_string_match(matches->str(1))
    };
}

NumStringMatch get_last_num_string_match(std::string line) {
    std::regex number_regex(
        "(?=(one|two|three|four|five|six|seven|eight|nine))");
    auto matches = std::sregex_iterator(line.begin(), line.end(), number_regex);
    auto end_sequence_iterator = std::sregex_iterator();

    if (matches == end_sequence_iterator)
        return { -1, 0 };  // no matches

    auto last_match = matches;
    while (matches != end_sequence_iterator) last_match = matches++;

    return {
        static_cast<int>(last_match->position()),
        get_value_of_num_string_match(last_match->str(1))
    };
}

int get_calibration_value_for_line(std::string line) {
    int first = 0, second = 0, line_length = line.length();
    int first_index = -1, second_index = line.length();
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
            second_index = back_index;
            second_not_set = false;
        }

        // Optimisations
        if (!first_not_set && !second_not_set) break;
        if (back_index < first_index) break;
    }

    auto first_num_string = get_first_num_string_match(line);
    auto last_num_string = get_last_num_string_match(line);

    if (first_not_set || first_num_string.index < first_index)
        first = first_num_string.value;

    if (second_not_set || last_num_string.index > second_index)
        second = last_num_string.value;

    return first*10 + second;
}

int main() {
    /* Tests */
    typedef struct { std::string input; int expected; } testcase;
    std::vector<testcase> testcases {
        { "two1nine", 0},
        { "eightwothree", 0},
        { "abcone2threexyz", 3},
        { "xtwone3four", 1},
        { "4nineeightseven2", 1},
        { "zoneight234", 1},
        { "7pqrstsixteen", 6},
        { "7pqrstddddteen", 14},
    };

    std::for_each(testcases.begin(), testcases.end(), [] (testcase tcase) {
        int output = get_first_num_string_match(tcase.input).index;
        std::cout <<
            "Testcase " + tcase.input + ": Is " + std::to_string(output) +
            ", Should be " + std::to_string(tcase.expected) + "\n";
        assert(output == tcase.expected);
    });

    testcases.clear();
    testcases = {
        { "two1nine", 4},
        { "eightwothree", 7},
        { "abcone2threexyz", 7},
        { "xtwone3four", 7},
        { "4nineeightseven2", 10},
        { "zoneight234", 3},
        { "7pqrstsixteen", 6},
        { "7pqrstddddteen", -1},
    };

    std::for_each(testcases.begin(), testcases.end(), [] (testcase tcase) {
        auto output = get_last_num_string_match(tcase.input).index;
        std::cout <<
            "Testcase " + tcase.input + ": Is " + std::to_string(output) +
            ", Should be " + std::to_string(tcase.expected) + "\n";
        assert(output == tcase.expected);
    });

    testcases.clear();
    testcases = {
        { "two1nine", 29},
        { "eightwothree", 83},
        { "abcone2threexyz", 13},
        { "xtwone3four", 24},
        { "4nineeightseven2", 42},
        { "zoneight234", 14},
        { "7pqrstsixteen", 76},
        { "pcg91vqrfpxxzzzoneightzt", 98},
    };

    std::for_each(testcases.begin(), testcases.end(), [] (testcase tcase) {
        auto output = get_calibration_value_for_line(tcase.input);
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
