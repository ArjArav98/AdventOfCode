#include<iostream>
#include<string>
#include<fstream>
#include<array>
#include<algorithm>

int main() {
    std::ifstream file;
    file.open("input/day10-input.txt", std::ios::in);

    int register_value = 1;
    int cycle = 0;
    
    int total_signal_strength = 0;
    std::array<int,6> special_cycles = {20, 60, 100, 140, 180, 220};

    while(!file.eof()) {
        std::string command;
        file>>command;

        int add_value = 0, times_to_loop = 0;
        
        if (command == "noop") {
            times_to_loop = 1;
        }
        else if (command == "addx") {
            file>>add_value;
            times_to_loop = 2;
        }

        for (int i=0; i<times_to_loop; i++, cycle++) {
            auto matching_special_cycle = std::find_if(
                special_cycles.begin(),
                special_cycles.end(),
                [cycle] (int special_cycle) { return (cycle + 1) == special_cycle; }
            );

            if (matching_special_cycle != special_cycles.end()) {
                total_signal_strength += ((cycle + 1) * register_value);
            }
        }

        register_value += add_value;
    }

    std::cout<<total_signal_strength<<std::endl;

    return 0;
}

