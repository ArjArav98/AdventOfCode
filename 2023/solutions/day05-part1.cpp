/* Copyright 2023 Arjun Aravind */
#include<cassert>
#include<cstdint>
#include<iostream>
#include<fstream>
#include<numeric>
#include<locale>
#include<string>
#include<stdexcept>
#include<vector>
#include<algorithm>

#include"../include/aoc.h"

class Rule {
  public: // NOLINT
    uint64_t range_start;
    uint64_t range_length;
    uint64_t conversion_constant;

    Rule(
        uint64_t range_start,
        uint64_t range_length,
        uint64_t conversion_constant
    ) {
        this->range_start = range_start;
        this->range_length = range_length;
        this->conversion_constant = conversion_constant;
    }

    bool operator==(const Rule rule) const {
        return (
            this->range_start == rule.range_start ||
            this->range_length == rule.range_length ||
            this->conversion_constant == rule.conversion_constant);
    }

    bool number_in_rule_range(uint64_t number) {
        return (
            number >= range_start &&
            (number <= (range_start+range_length)));
    }
};

class Convertor {
  public: // NOLINT
    std::vector<Rule> rules;

    explicit Convertor(std::vector<Rule> rules) {
        this->rules = rules;
    }
    Convertor() {}

    bool operator==(const Convertor convertor) const {
        return (this->rules == convertor.rules);
    }

    void add_rule(Rule rule) {
        rules.push_back(rule);
    }

    uint64_t convert(uint64_t number) {
        for (auto rule : rules) {
            if (rule.number_in_rule_range(number))
                return number - rule.conversion_constant;
        }
        return number;
    }
};

std::vector<uint64_t> extract_numbers(std::string line) {
    auto numbers = aoc::split(aoc::split(line, ": ").at(1), " ");
    std::vector<uint64_t> actual_numbers;
    actual_numbers.resize(numbers.size());

    std::transform(
        numbers.begin(),
        numbers.end(),
        actual_numbers.begin(),
        [](std::string number) -> uint64_t {
            return std::stoull(number);
        });

    return actual_numbers;
}

std::vector<Convertor> extract_convertors(std::vector<std::string> input) {
    bool same_convertor = true;
    Convertor convertor = Convertor();
    std::vector<Convertor> convertors;

    for (auto line : input) {
        if (line.length() == 0) continue;
        if (std::isalpha(line.at(0))) {
            convertors.push_back(convertor);
            convertor = Convertor();
            continue;
        }

        auto splits = aoc::split(line, " ");
        if (splits.size() != 3) throw std::invalid_argument(line);

        convertor.add_rule(Rule(
            std::stoull(splits.at(1)),
            std::stoull(splits.at(2)),
            std::stoull(splits.at(1)) - std::stoull(splits.at(0))));
    }

    convertors.push_back(convertor);
    return convertors;
}

uint64_t convert_number(uint64_t number, std::vector<Convertor> convertors) {
    return std::reduce(
        convertors.begin(),
        convertors.end(),
        number,
        [] (uint64_t previous_num, Convertor convertor) -> uint64_t {
            return convertor.convert(previous_num);
        });
}

void test_extract_convertors() {
    typedef struct {
        std::vector<std::string> input;
        std::vector<Convertor> expected;
    } Testcase;

    std::vector<Testcase> testcases {
        {
            {
                "seed-to-soil map:",
                "50 98 2",
                "52 50 48",
                "soil-to-fertilizer map:",
                "0 15 37",
                "37 52 2",
                "39 0 15",
                "fertilizer-to-water map:",
                "49 53 8",
                "0 11 42",
                "42 0 7",
                "57 7 4",
                "water-to-light map:",
                "88 18 7",
                "18 25 70",
                "light-to-temperature map:",
                "45 77 23",
                "81 45 19",
                "68 64 13",
                "temperature-to-humidity map:",
                "0 69 1",
                "1 0 69",
                "humidity-to-location map:",
                "60 56 37",
                "56 93 4",
            },
            {
                Convertor(),
                Convertor({
                    Rule(98, 2, 48),
                    Rule(50, 48, -2)
                }),
                Convertor({
                    Rule(15, 37, 15),
                    Rule(52, 2, 15),
                    Rule(0, 15, -39)
                }),
                Convertor({
                    Rule(53, 8, 4),
                    Rule(11, 42, 11),
                    Rule(0, 7, -42),
                    Rule(7, 4, -50)
                }),
                Convertor({
                    Rule(18, 7, -70),
                    Rule(25, 70, 7)
                }),
                Convertor({
                    Rule(77, 23, 32),
                    Rule(45, 19, 36),
                    Rule(64, 13, -4)
                }),
                Convertor({
                    Rule(69, 1, 69),
                    Rule(0, 69, -1)
                }),
                Convertor({
                    Rule(56, 37, -4),
                    Rule(93, 4, 37)
                })
            }
        }
    };

    std::cout << "test_extract_convertors\n";
    std::for_each(testcases.begin(), testcases.end(), [](Testcase tcase) {
        std::cout << "Testing " << ": ";
        auto convertors = extract_convertors(tcase.input);
        assert(convertors == tcase.expected);
        std::cout << "Passed\n";
    });
}

void test_convert_number() {
    typedef struct {
        std::vector<Convertor> convertors;
        uint64_t number;
        uint64_t expected;
    } Testcase;

    std::vector<Convertor> convertors = {
        Convertor(),
        Convertor({
            Rule(98, 2, 48),
            Rule(50, 48, -2)
        }),
        Convertor({
            Rule(15, 37, 15),
            Rule(52, 2, 15),
            Rule(0, 15, -39)
        }),
        Convertor({
            Rule(53, 8, 4),
            Rule(11, 42, 11),
            Rule(0, 7, -42),
            Rule(7, 4, -50)
        }),
        Convertor({
            Rule(18, 7, -70),
            Rule(25, 70, 7)
        }),
        Convertor({
            Rule(77, 23, 32),
            Rule(45, 19, -36),
            Rule(64, 13, -4)
        }),
        Convertor({
            Rule(69, 1, 69),
            Rule(0, 69, -1)
        }),
        Convertor({
            Rule(56, 37, -4),
            Rule(93, 4, 37)
        })
    };

    std::vector<Testcase> testcases {
        { convertors, 79, 82 },
        { convertors, 14, 43 },
        { convertors, 55, 86 },
        { convertors, 13, 35 },
    };

    std::cout << "test_convert_number\n";
    std::for_each(testcases.begin(), testcases.end(), [](Testcase tcase) {
        std::cout << "Testing " << tcase.number << ": ";
        auto result = convert_number(tcase.number, tcase.convertors);
        assert(result == tcase.expected);
        std::cout << "Passed\n";
    });
}

void test_extract_numbers() {
    typedef struct {
        std::string input;
        std::vector<uint64_t> expected;
    } Testcase;

    std::vector<Testcase> testcases {
        {
            "seeds: 79 14 55 13",
            { 79, 14, 55, 13 }
        },
        {
            "seeds: 629551616 310303897 265998072 58091853 3217788227"
            " 563748665 2286940694 820803307 1966060902 108698829 190045874"
            " 3206262 4045963015 223661537 1544688274 293696584 1038807941"
            " 31756878 1224711373 133647424",
            {
                629551616, 310303897, 265998072, 58091853,
                3217788227, 563748665, 2286940694, 820803307,
                1966060902, 108698829, 190045874, 3206262,
                4045963015, 223661537, 1544688274, 293696584,
                1038807941, 31756878, 1224711373, 133647424
            }
        }
    };

    std::cout << "test_extract_numbers\n";
    std::for_each(testcases.begin(), testcases.end(), [](Testcase tcase) {
        std::cout << "Testing " << ": ";
        assert(extract_numbers(tcase.input) == tcase.expected);
        std::cout << "Passed\n";
    });
}

int main() {
    test_extract_convertors();
    test_convert_number();
    test_extract_numbers();

    std::fstream file;
    file.open("input/day05.txt", std::ios::in);

    std::vector<std::string> input;

    do {
        std::string line;
        std::getline(file, line);
        input.push_back(line);
    } while (file.is_open() && !file.eof());

    std::vector<std::string> filtered_input;
    filtered_input.resize(input.size() - 2);

    std::copy_if(
        input.begin() + 2,  // remove headers
        input.end(),
        filtered_input.begin(),
        [](std::string line) -> bool {
            return line.length() > 1;
        });

    auto numbers_to_convert = extract_numbers(input.at(0));
    auto convertors = extract_convertors(filtered_input);

    std::vector<uint64_t> results;
    results.resize(numbers_to_convert.size());

    std::transform(
        numbers_to_convert.begin(),
        numbers_to_convert.end(),
        results.begin(),
        [&convertors](uint64_t number) -> uint64_t {
            return convert_number(number, convertors);
        });

    auto lowest = std::min_element(results.begin(), results.end());

    std::cout << *lowest << std::endl;

    return 0;
}
