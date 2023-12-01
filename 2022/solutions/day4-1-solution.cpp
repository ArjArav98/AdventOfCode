#include<iostream>
#include<string>
#include<fstream>

struct Range {
    int start;
    int end;
};

bool one_range_contains_the_other(Range r1, Range r2) {
    return (
        ((r1.start <= r2.start) && (r1.end >= r2.end)) ||
        ((r2.start <= r1.start) && (r2.end >= r1.end))
    );
}

int main() {
    std::ifstream file;
    file.open("input/day4-1-input.txt", std::ios::in);

    int ranges_which_contain_other_ranges = 0;

    while(!file.eof()) {
        std::string line;
        file>>line;

        std::size_t first_dash = line.find('-');
        std::size_t last_dash = line.rfind('-');
        std::size_t comma = line.find(',');

        if (comma == std::string::npos) continue;

        Range r1 = {
            std::stoi(line.substr(0,first_dash)),
            std::stoi(line.substr(first_dash+1,comma))
        };
        Range r2 = {
            std::stoi(line.substr(comma+1,last_dash)),
            std::stoi(line.substr(last_dash+1))
        };

        if (one_range_contains_the_other(r1,r2)) ranges_which_contain_other_ranges++;
    }

    std::cout<<ranges_which_contain_other_ranges<<std::endl;

    return 0;
}
