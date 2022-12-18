#include<algorithm>
#include<iostream>
#include<string>
#include<vector>
#include<list>
#include<fstream>
#include<functional>
#include<utility>
#include<numeric>

/* --------- */
/* UTILITIES */
/* --------- */

std::list<long> comma_list_to_long_list(std::string line) {
    std::list<long> long_list;
    auto length = line.length();
    auto starting_char = 0;
    
    while(true) {
        auto comma_pos = line.find(",", starting_char);
        auto num_str = line.substr(starting_char, comma_pos);
        long_list.push_back(std::stol(num_str, nullptr, 10));
        starting_char = comma_pos + 1;

        if (comma_pos == std::string::npos) break;
    }

    return long_list;
}

/* ------- */
/* CLASSES */
/* ------- */

class Monkey {
    public:
        std::list<long> worry_levels;
    
        char worry_level_increase_operator;
        long worry_level_increase_operand;

        long throw_test_operand;

        int monkey_to_throw_to_if_test_passes;
        int monkey_to_throw_to_if_test_fails;

        int num_of_items_inspected;

    public:
        Monkey(std::vector<std::string> lines) {
            parse_from_input(lines);
        }

        void parse_from_input(std::vector<std::string> lines) {
            for(auto line: lines) {
                if (line.find("Starting items") != std::string::npos) {
                    auto list_starting_pos = line.find("items:") + 7;
                    worry_levels =
                        comma_list_to_long_list(line.substr(list_starting_pos));
                }
                else if (line.find("Operation") != std::string::npos) {
                    auto operator_pos = line.find("new = old") + 10;
                    auto operand_pos = operator_pos + 2;

                    auto _operand = line.substr(operand_pos);
                    worry_level_increase_operator =
                        (_operand == "old")? '^' : line.at(operator_pos);
                    worry_level_increase_operand =
                        (_operand == "old")? 0 : std::stoi(_operand, nullptr, 10);
                }
                else if (line.find("Test") != std::string::npos) {
                    auto operand_pos = line.find("divisible by") + 13;
                    throw_test_operand = std::stol(line.substr(operand_pos), nullptr, 10); 
                }
                else if (line.find("If true") != std::string::npos) {
                    auto monkey_num_pos = line.find("throw to monkey") + 16;
                    monkey_to_throw_to_if_test_passes =
                        std::stoi(line.substr(monkey_num_pos), nullptr, 10);
                }
                else if (line.find("If false") != std::string::npos) {
                    auto monkey_num_pos = line.find("throw to monkey") + 16;
                    monkey_to_throw_to_if_test_fails =
                        std::stoi(line.substr(monkey_num_pos), nullptr, 10);
                }
            }
        
            num_of_items_inspected = 0;
        }

        void do_worry_level_calculations(long lcm) {
            std::for_each(
                worry_levels.begin(),
                worry_levels.end(),
                [this] (long &worry_level) -> void {
                    switch (this->worry_level_increase_operator) {
                        case '*': worry_level *= this->worry_level_increase_operand; break;
                        case '+': worry_level += this->worry_level_increase_operand; break;
                        case '^': worry_level *= worry_level; break;
                    }
                }
            );
           
            /* We want to find a way to manage worry levels.
             * One way to do this would be to set the worry_level
             * to the remainder of the LCM. The LCM is the lowest-
             * common-multiple for all the throw_test_operand variables
             * of all classes.
             *
             * We do this so that the worry levels retain their
             * properties of being divisible/non-divisible by the other
             * throw_test_operand variables. 
             * */
            std::for_each(
                worry_levels.begin(),
                worry_levels.end(),
                [this, &lcm] (long &worry_level) -> void {
                    worry_level = (worry_level % lcm) + lcm;
                }
            );
        }

        std::pair<long, int> inspect_and_pop_front() {
            auto worry_level = worry_levels.front();

            int monkey_to_throw_to = (
                worry_level % throw_test_operand == 0
            )? monkey_to_throw_to_if_test_passes : monkey_to_throw_to_if_test_fails;

            worry_levels.pop_front();
            num_of_items_inspected++;

            return std::make_pair(worry_level, monkey_to_throw_to);
        }
};

/* --------- */
/* UTILITIES */
/* --------- */

void print_all_monkeys(Monkey monkey) {
    std::cout<<"Worry level items ";
    for (auto num: monkey.worry_levels) {
        std::cout<<num<<" ";
    }
    std::cout<<std::endl;

    std::cout<<"Worry increase stuff "<<monkey.worry_level_increase_operator;
    std::cout<<" "<<monkey.worry_level_increase_operand<<std::endl;

    std::cout<<"Test "<<monkey.throw_test_operand<<std::endl;
    std::cout<<"Test result "<<monkey.monkey_to_throw_to_if_test_passes;
    std::cout<<" "<<monkey.monkey_to_throw_to_if_test_fails<<std::endl;
    std::cout<<std::endl;
}

long lowest_common_multiple(std::vector<Monkey> monkeys) {
    std::vector<long> all_throw_test_operands(monkeys.size());
    std::transform(
        monkeys.begin(),
        monkeys.end(),
        all_throw_test_operands.begin(),
        [] (Monkey monkey) -> long { return monkey.throw_test_operand; }
    );

    auto largest_elem = std::max_element(
        all_throw_test_operands.begin(),
        all_throw_test_operands.end()
    );

    long multiple = 0;

    while(true) {
        multiple += *largest_elem;
        auto divisible_by_all = std::all_of(
            all_throw_test_operands.begin(),
            all_throw_test_operands.end(),
            [multiple] (long number) -> bool {
                return multiple % number == 0;
            }
        );
        if (divisible_by_all) break;
    }

    return multiple;
}

int main() {
    std::ifstream file;
    file.open("input/day11-input.txt", std::ios::in);

    std::vector<Monkey> monkeys;

    /* We parse the information from the file. */
    while(!file.eof()) {
        std::vector<std::string> monkey_lines;

        while(true) {
            std::string line;
            std::getline(file, line);
            if (line.length() == 0) break;
            monkey_lines.push_back(line);
        }

        if (file.eof()) break;

        auto monkey = Monkey(monkey_lines);
        monkeys.push_back(monkey);
    }

    long num_of_rounds = 10000;
    long lcm = lowest_common_multiple(monkeys);

    /* We do the iteration for the specified number of rounds. */
    for (long i=0; i<num_of_rounds; i++) {

        /* We iterate over all the monkeys. */
        for (auto &monkey: monkeys) {
            monkey.do_worry_level_calculations(lcm);
            auto size = monkey.worry_levels.size();

            for (int i=0; i<size; i++) {
                auto pop = monkey.inspect_and_pop_front();
                monkeys.at(pop.second).worry_levels.push_back(pop.first);
            }
        }
    }

    /* We sort the monkeys according to the items they've inspected. */
    std::sort(
        monkeys.begin(),
        monkeys.end(),
        [] (Monkey monkey1, Monkey monkey2) -> bool {
            return monkey1.num_of_items_inspected > monkey2.num_of_items_inspected;
        }
    );

    long long product_of_top_two_monkeys = (
        (long long) monkeys.at(0).num_of_items_inspected *
        (long long) monkeys.at(1).num_of_items_inspected
    );

    std::cout<<product_of_top_two_monkeys<<std::endl;

    return 0;
}
