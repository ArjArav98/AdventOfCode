/* Copyright 2023 Arjun Aravind */

#include<cassert>
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<algorithm>

#include"A-answer.h"

void test_get_game_data_from_line() {
    typedef struct { std::string input; aoc::Game expected; } testcase;
    std::vector<testcase> testcases {
        {
            "Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green",
            aoc::Game(
                1,
                std::vector<aoc::GameSubset> {
                    aoc::GameSubset(4, 3, 0),
                    aoc::GameSubset(1, 6, 2),
                    aoc::GameSubset(0, 0, 2)
                }
            )
        },
        {
            "Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue",
            aoc::Game(
                2,
                std::vector<aoc::GameSubset> {
                    aoc::GameSubset(0, 1, 2),
                    aoc::GameSubset(1, 4, 3),
                    aoc::GameSubset(0, 1, 1)
                }
            )
        },
        {
            "Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green;"
            " 5 green, 1 red",
            aoc::Game(
                3,
                std::vector<aoc::GameSubset> {
                    aoc::GameSubset(20, 6, 8),
                    aoc::GameSubset(4, 5, 13),
                    aoc::GameSubset(1, 0, 5)
                }
            )
        },
        {
            "Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red;"
            " 3 green, 15 blue, 14 red",
            aoc::Game(
                4,
                std::vector<aoc::GameSubset> {
                    aoc::GameSubset(3, 6, 1),
                    aoc::GameSubset(6, 0, 3),
                    aoc::GameSubset(14, 15, 3)
                }
            )
        },
        {
            "Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green",
            aoc::Game(
                5,
                std::vector<aoc::GameSubset> {
                    aoc::GameSubset(6, 1, 3),
                    aoc::GameSubset(1, 2, 2),
                }
            )
        },
    };

    std::for_each(testcases.begin(), testcases.end(), [] (testcase tcase) {
        auto output = aoc::get_game_data_from_line(tcase.input);
        std::cout << "Testcase " << tcase.input << ": Is " << output;
        std::cout << ", Should be " << tcase.expected << "\n";
        assert(output == tcase.expected);
    });
}

int main() {
    return 0;
}
