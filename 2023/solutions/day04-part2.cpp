/* Copyright 2023 Arjun Aravind */
#include<cassert>
#include<cmath>
#include<iostream>
#include<fstream>
#include<array>
#include<string>
#include<vector>
#include<numeric>
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

long get_intersecting_elem_score(std::vector<int> intersecting_count_for_line) { // NOLINT
    std::vector<int> line_repeats;
    for (auto line : intersecting_count_for_line) line_repeats.push_back(1);

    int num_of_elems = intersecting_count_for_line.size();

    for (int line=0; line < num_of_elems; line++) {
        int intersecting_count = intersecting_count_for_line.at(line);
        for (
            int i = 1;
            (i <= intersecting_count) && ((line+i) < num_of_elems);
            i++
        )
            line_repeats[line+i] += line_repeats[line];
    }

    return std::accumulate(line_repeats.begin(), line_repeats.end(), 0);
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

void test_get_intersecting_elem_score() {
    typedef struct {
        std::vector<int> intersecting_count_for_line;
        int expected;
    } Testcase;

    std::vector<Testcase> testcases {
        { {4, 2, 2, 1, 0, 0}, 30 },
        { {2, 4, 3}, 7 }
    };

    std::cout << "test_get_intersection_elems\n";
    std::for_each(testcases.begin(), testcases.end(), [](Testcase tcase) {
        std::cout << "Testing " << tcase.intersecting_count_for_line << ": ";
        assert(get_intersecting_elem_score(
            tcase.intersecting_count_for_line) == tcase.expected);
        std::cout << "Passed\n";
    });
}

int main() {
    test_extract_number_sets();
    test_get_intersection_elems();
    test_get_intersecting_elem_score();

    std::fstream file;
    file.open("input/day04.txt", std::ios::in);

    int line_num = 0;
    std::vector<int> intersecting_count_for_line;

    do {
        std::string line;
        std::getline(file, line);

        if (file.eof()) continue;

        auto number_sets = extract_number_sets(line);

        std::set<int> intersection_elems;
        std::set_intersection(
            number_sets.first.begin(),
            number_sets.first.end(),
            number_sets.second.begin(),
            number_sets.second.end(),
            std::inserter(intersection_elems, intersection_elems.begin()));

        intersecting_count_for_line.push_back(intersection_elems.size());
        line_num++;
    } while (file.is_open() && !file.eof());

    std::cout <<
        get_intersecting_elem_score(intersecting_count_for_line) << std::endl;

    return 0;
}
