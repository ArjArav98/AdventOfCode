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

bool number_adjacent_to_symbol(
    aoc::Coord num_coord,
    int num_length,
    aoc::Coord symbol_coord
) {
    for (int iter=0; iter < num_length; iter++) {
        std::vector<std::pair<int, int>> operations = {
            std::make_pair(-1, 0), std::make_pair(1, 0),
            std::make_pair(-1, -1), std::make_pair(-1, 1),
            std::make_pair(1, -1), std::make_pair(1, 1)
        };
        // Don't check right/left if digits exist there
        if (iter+1 == num_length) operations.push_back(std::make_pair(0, 1));
        if (iter == 0) operations.push_back(std::make_pair(0, -1));

        bool is_part_num = std::any_of(
            operations.begin(),
            operations.end(),
            [num_coord, symbol_coord, iter](auto op){
                aoc::Coord new_num_coord(
                    num_coord.row + op.first, num_coord.col + iter + op.second);
                return new_num_coord == symbol_coord;
            });

        if (is_part_num) return true;
    }

    return false;
}

std::vector<int> get_nums_adjacent_to_gear(
    aoc::Coord gear_coord,
    Index* number_index
) {
    std::vector<int> rows {
        gear_coord.row,
        gear_coord.row - 1,
        gear_coord.row + 1
    };

    std::vector<int> numbers;

    for (auto row : rows) {
        try {
            for (auto col : number_index->at(row)) {
                auto num_coord = aoc::Coord(row, col.first);
                int length = std::to_string(col.second).length();
                if (number_adjacent_to_symbol(num_coord, length, gear_coord))
                    numbers.push_back(col.second);
            }
        } catch (std::out_of_range e) {}
    }

    return numbers;
}

void test_update_number_index() {
    typedef struct { int row; int col; int expected_number; } testcase;
    std::vector<testcase> testcases {
        { 0, 4, 2 },
        { 0, 6, 755 },
        { 4, 1, 664 },
        { 4, 5, 598 },
    };

    Index number_index;

    update_number_index("....2.755.", 0, &number_index);
    update_number_index("...$.*....", 2, &number_index);
    update_number_index(".664.598..", 4, &number_index);

    std::cout << "test_update_number_index\n";
    std::for_each(testcases.begin(), testcases.end(), [&](testcase tcase) {
        std::cout << "Testing (" << tcase.row << "," << tcase.col
        << ") contains " << tcase.expected_number << ": ";
        assert(number_index[tcase.row][tcase.col] == tcase.expected_number);
        std::cout << "Passed\n";
    });
}

void test_number_adjacent_to_symbol() {
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

    typedef struct {
        aoc::Coord num_coord; int num_length;
        aoc::Coord symbol_coord; bool expected_output; } testcase;
    std::vector<testcase> testcases {
        { {0, 0}, 3, {1, 3}, true },
        { {2, 2}, 2, {1, 3}, true },
        { {7, 7}, 3, {8, 6}, true },
        { {9, 6}, 3, {8, 6}, true },
        { {9, 0}, 1, {8, 6}, false },
    };

    std::cout << "test_number_adjacent_to_symbol\n";
    std::for_each(testcases.begin(), testcases.end(), [&](testcase tcase) {
        std::cout << "Testing " << tcase.num_coord << " is adjacent to "
        << tcase.symbol_coord << " is " << tcase.expected_output << ": ";
        bool number_is_adjacent = number_adjacent_to_symbol(
            tcase.num_coord, tcase.num_length, tcase.symbol_coord);
        assert(number_is_adjacent == tcase.expected_output);
        std::cout << "Passed\n";
    });
}

void test_get_nums_adjacent_to_gear() {
    Index number_index;

    update_number_index("....2.755.", 0, &number_index);
    update_number_index("...$.*....", 1, &number_index);
    update_number_index(".664.598..", 2, &number_index);

    typedef struct {
        aoc::Coord gear_coord; std::vector<int> expected_output; } testcase;
    std::vector<testcase> testcases {
        { aoc::Coord(1, 5), std::vector<int> {2, 598, 755} }
    };

    std::cout << "test_get_nums_adjacent_to_gear\n";
    std::for_each(testcases.begin(), testcases.end(), [&](testcase tcase) {
        std::cout << "Testing nums adjacent to " << tcase.gear_coord <<
        " are " << tcase.expected_output << ": ";
        auto adjacent_nums =
            get_nums_adjacent_to_gear(tcase.gear_coord, &number_index);
        std::sort(adjacent_nums.begin(), adjacent_nums.end());
        std::sort(tcase.expected_output.begin(), tcase.expected_output.end());
        assert(adjacent_nums == tcase.expected_output);
        std::cout << "Passed\n";
    });
}

int main() {
    test_update_number_index();
    test_number_adjacent_to_symbol();
    test_get_nums_adjacent_to_gear();

    Index number_index;
    std::vector<std::string> schematic;
    std::fstream file;

    int line_num = 0;
    long long product_sum = 0; // NOLINT
    file.open("input/day03.txt", std::ios::in);

    do {
        std::string line;
        std::getline(file, line);

        update_number_index(line, line_num, &number_index);
        schematic.push_back(line);

        line_num++;
    } while (file.is_open() && !file.eof());

    for (int row_iter=0; row_iter < schematic.size(); row_iter++) {
        auto row = schematic.at(row_iter);
        for (int col_iter=0; col_iter < row.length(); col_iter++) {
            char character = row.at(col_iter);
            if (character != '*') continue;

            auto adjacent_nums = get_nums_adjacent_to_gear(
                aoc::Coord(row_iter, col_iter),
                &number_index);

            if (adjacent_nums.size() != 2) continue;

            long long product = adjacent_nums[0]*adjacent_nums[1]; // NOLINT
            product_sum += product;
        }
    }

    std::cout << product_sum << std::endl;

    return 0;
}
