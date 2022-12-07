#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<numeric>
#include<algorithm>

int main() {
    std::ifstream file;
    file.open("input/day1-1-input.txt", std::ios::in);

    std::vector<long> totals;
    long total = 0;

    while(!file.eof()) {
        std::string line;
        std::getline(file, line);
       
        if (line.length() == 0) {
            totals.push_back(total);
            total=0;
        }
        else {
            total += std::stol(line,nullptr,10);
        }
    }

    std::vector<long> greatest_totals;

    for(int i=0; i<3; i++) {
        auto compare = [&] (long current_greatest, long num) -> long {
            auto element_which_matches_num = std::find(
                greatest_totals.begin(),
                greatest_totals.end(),
                num
            );

            if (element_which_matches_num != greatest_totals.end()) return current_greatest;
            return num > current_greatest ? num : current_greatest;
        };

        long greatest_total = std::reduce(totals.begin(), totals.end(), 0, compare);
        greatest_totals.push_back(greatest_total);
    }

    long total_of_greatest_totals = std::accumulate(
        greatest_totals.begin(),
        greatest_totals.end(),
        0
    );

    std::cout<<total_of_greatest_totals<<std::endl;
    file.close();
    return 0;
}
