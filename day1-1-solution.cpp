#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<numeric>

int main() {
    std::ifstream file;
    file.open("day1-1-input.txt", std::ios::in);

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

    auto compare = [] (long num1, long num2) -> long {
        return num1 > num2 ? num1 :  num2;
    };

    long greatest_total = std::reduce(totals.begin(), totals.end(), 0, compare);

    std::cout<<greatest_total<<std::endl;
    file.close();
    return 0;
}
