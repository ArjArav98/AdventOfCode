/* Copyright 2023 Arjun Aravind */
#include<cassert>
#include<cmath>
#include<iostream>
#include<fstream>
#include<array>
#include<string>
#include<vector>
#include<set>
#include<stdexcept>
#include<regex>
#include<algorithm>

#include"../include/aoc.h"

typedef struct {
    std::set<int> first;
    std::set<int> second;
} NumberSets;

std::set<int> extract_number_set(std::string line_section) {
    std::regex number_regex("[0-9]+");
    auto matches_begin = std::sregex_iterator(
        line_section.begin(),
        line_section.end(),
        number_regex);
    auto matches_end = std::sregex_iterator();

    std::set<int> number_set;

    std::for_each(
        matches_begin,
        matches_end,
        [&number_set](auto match) -> void {
            number_set.insert(std::stoi(match.str()));
        });

    return number_set;
}

NumberSets extract_number_sets(std::string line) {
    auto sections = aoc::split(line, ":");

    if (sections.size() != 2) return { {}, {} };

    auto number_section = sections.at(1);
    auto number_sections = aoc::split(number_section, "|");

    if (number_sections.size() != 2) return { {}, {} };

    return {
        extract_number_set(number_sections.at(0)),
        extract_number_set(number_sections.at(1))
    };
}

std::set<int> get_intersection_elems(std::set<int> set1, std::set<int> set2) {
    std::set<int> intersection_elems;
    std::set_intersection(
        set1.begin(),
        set1.end(),
        set2.begin(),
        set2.end(),
        std::inserter(intersection_elems, intersection_elems.begin()));
    return intersection_elems;
}

void test_extract_number_sets() {
    typedef struct { std::string line; NumberSets expected; } Testcase;
    std::vector<Testcase> testcases {
        {
            "Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53",
            { {41, 48, 83, 86, 17}, {83, 86, 6, 31, 17, 9, 48, 53} }
        },
        {
            "Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19",
            { {13, 32, 20, 16, 61}, {61, 30, 68, 82, 17, 32, 24, 19} }
        },
        {
            "Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1",
            { {1, 21, 53, 59, 44}, {69, 82, 63, 72, 16, 21, 14, 1} }
        },
        {
            "Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83",
            { {41, 92, 73, 84, 69}, {59, 84, 76, 51, 58, 5, 54, 83} }
        },
        {
            "Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36",
            { {87, 83, 26, 28, 32}, {88, 30, 70, 12, 93, 22, 82, 36} }
        },
        {
            "Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11",
            { {31, 18, 13, 56, 72}, {74, 77, 10, 23, 35, 67, 36, 11} }
        }
    };

    std::cout << "test_extract_number_sets\n";
    std::for_each(testcases.begin(), testcases.end(), [](Testcase tcase) {
        std::cout << "Testing " << tcase.line << ": ";
        NumberSets num_sets = extract_number_sets(tcase.line);

        assert(num_sets.first == tcase.expected.first);
        assert(num_sets.second == tcase.expected.second);

        std::cout << "Passed\n";
    });
}

void test_get_intersection_elems() {
    typedef struct {
        std::set<int> set1;
        std::set<int> set2;
        std::set<int> expected;
    } Testcase;

    std::vector<Testcase> testcases {
        {
            {41, 48, 83, 86, 17}, {83, 86, 6, 31, 17, 9, 48, 53},
            {48, 83, 86, 17}
        },
        {
            {13, 32, 20, 16, 61}, {61, 30, 68, 82, 17, 32, 24, 19},
            {32, 61}
        },
    };

    std::cout << "test_get_intersection_elems\n";
    std::for_each(testcases.begin(), testcases.end(), [](Testcase tcase) {
        std::cout << "Testing " << tcase.set1 << " and " << tcase.set2 << ": ";
        assert(
            get_intersection_elems(tcase.set1, tcase.set2) == tcase.expected);

        std::cout << "Passed\n";
    });
}

int main() {
    test_extract_number_sets();
    test_get_intersection_elems();

    std::fstream file;
    file.open("input/day04.txt", std::ios::in);

    double total_score = 0;

    do {
        std::string line;
        std::getline(file, line);

        auto number_sets = extract_number_sets(line);
        std::set<int> intersection_elems;
        std::set_intersection(
            number_sets.first.begin(),
            number_sets.first.end(),
            number_sets.second.begin(),
            number_sets.second.end(),
            std::inserter(intersection_elems, intersection_elems.begin()));

        if (intersection_elems.size() == 0) continue;

        double score = pow(2, intersection_elems.size()-1);
        total_score += score;
    } while (file.is_open() && !file.eof());

    std::cout << total_score << std::endl;

    return 0;
}
