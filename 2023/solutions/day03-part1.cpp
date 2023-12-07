/* Copyright 2023 Arjun Aravind */
#include<cassert>
#include<iostream>
#include<fstream>
#include<string>
#include<stdexcept>
#include<vector>
#include<map>
#include<regex>
#include<algorithm>

#include"../include/aoc.h"

// { 0: { 1: 213, 2: 213}, 1: { 5: 132, 6: 132 } .. }
typedef std::map<int, std::map<int, int>> Index;
typedef struct { int row; int col; } Coord;

void update_number_index(std::string line, int row_num, Index* number_index) {
    std::regex number_regex("[0-9]+");
    auto matches_begin = std::sregex_iterator(
        line.begin(),
        line.end(),
        number_regex);
    auto matches_end = std::sregex_iterator();

    std::for_each(
        matches_begin,
        matches_end,
        [row_num, number_index](auto match) -> void {
            (*number_index)[row_num][match.position()] = std::stoi(match.str());
        });
}

bool is_part_number(
    Coord coord,
    int length,
    std::vector<std::string> schematic
) {
    for (int iter=0; iter < length; iter++) {
        std::vector<std::pair<int, int>> operations = {
            std::make_pair(-1, 0), std::make_pair(1, 0),
            std::make_pair(-1, -1), std::make_pair(-1, 1),
            std::make_pair(1, -1), std::make_pair(1, 1)
        };
        // Don't check right/left if digits exist there
        if (iter+1 == length) operations.push_back(std::make_pair(0, 1));
        if (iter == 0) operations.push_back(std::make_pair(0, -1));

        bool is_part_num = std::any_of(
            operations.begin(),
            operations.end(),
            [&schematic, coord, iter](auto op){
                Coord ncoord = {
                    coord.row + op.first, coord.col + iter + op.second
                };
                try {
                    char value = schematic.at(ncoord.row).at(ncoord.col);
                    return !(std::isdigit(value) || (value == '.'));
                } catch (const std::out_of_range e) {
                    return false;
                }
            });

        if (is_part_num) return true;
    }

    return false;
}

void test_update_number_index() {
    typedef struct { int row; int col; int expected_number; } testcase;
    std::vector<testcase> testcases {
        { 0, 0, 467 },
        { 0, 5, 114 },
        { 2, 2, 35 },
        { 2, 6, 633 },
        { 4, 0, 617 },
    };

    Index number_index;

    update_number_index("467..114..", 0, &number_index);
    update_number_index("..35..633.", 2, &number_index);
    update_number_index("617*......", 4, &number_index);

    std::cout << "test_update_number_index\n";
    std::for_each(testcases.begin(), testcases.end(), [&](testcase tcase) {
        std::cout << "Testing (" << tcase.row << "," << tcase.col
        << ") contains " << tcase.expected_number << ": ";
        assert(number_index[tcase.row][tcase.col] == tcase.expected_number);
        std::cout << "Passed\n";
    });
}

void test_is_part_number() {
    std::vector<std::string> schematic {
        "467..114..",
        "...*......",
        "..35..633.",
        "......#...",
        "617*......",
        ".....+.58.",
        "..592.....",
        "......755.",
        "...$.*....",
        ".664.598.."
    };

    typedef struct { Coord coord; int length; bool expected_output; } testcase;
    std::vector<testcase> testcases {
        { {0, 0}, 3, true }, { {0, 5}, 3, false},
        { {2, 2}, 2, true }, { {2, 6}, 3, true},
        { {4, 0}, 3, true },
        { {6, 2}, 3, true }, { {7, 6}, 3, true},
        { {9, 1}, 3, true }, { {9, 5}, 3, true},
        { {5, 7}, 2, false }, { {8, 1}, 1, false}
    };

    std::cout << "test_is_part_number\n";
    std::for_each(testcases.begin(), testcases.end(), [&](testcase tcase) {
        std::cout << "Testing (" << tcase.coord.row << "," << tcase.coord.col
        << ") is a part number is " << tcase.expected_output << ": ";
        assert(is_part_number(tcase.coord, tcase.length, schematic)
            == tcase.expected_output);
        std::cout << "Passed\n";
    });
}

int main() {
    test_update_number_index();
    test_is_part_number();

    Index number_index;
    std::vector<std::string> schematic;
    std::fstream file;

    int line_num = 0, sum = 0;
    file.open("input/day03.txt", std::ios::in);

    do {
        std::string line;
        std::getline(file, line);

        update_number_index(line, line_num, &number_index);
        schematic.push_back(line);

        line_num++;
    } while (file.is_open() && !file.eof());

    for (auto row : number_index) {
        for (auto col : row.second) {
            int value = col.second;
            int length = std::to_string(col.second).length();
            bool is_part_num =
                is_part_number({ row.first, col.first }, length, schematic);
            sum += (is_part_num)? col.second : 0;
        }
    }

    std::cout << sum << std::endl;

    return 0;
}
