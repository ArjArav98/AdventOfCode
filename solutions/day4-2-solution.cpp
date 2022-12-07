#include<iostream>
#include<string>
#include<fstream>

struct Range {
    int start;
    int end;
};

bool ranges_overlap(Range r1, Range r2) {
    int length_of_r1 = r1.end - r1.start;
    int length_of_r2 = r2.end - r2.start;

    bool r1_overlaps_with_start_of_r2 = (
        (r1.start <= r2.start) &&
        ((r1.start + length_of_r1) >= r2.start)
    );
    if (r1_overlaps_with_start_of_r2) return true;

    bool r2_overlaps_with_start_of_r1 = (
        (r2.start <= r1.start) &&
        ((r2.start + length_of_r2) >= r1.start)
    );
    if (r2_overlaps_with_start_of_r1) return true;

    return false;
}

int main() {
    std::ifstream file;
    file.open("input/day4-1-input.txt", std::ios::in);

    int ranges_which_overlap = 0;

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

        if (ranges_overlap(r1,r2)) ranges_which_overlap++;
    }

    std::cout<<ranges_which_overlap<<std::endl;

    return 0;
}
