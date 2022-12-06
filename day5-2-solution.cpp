#include<iostream>
#include<fstream>
#include<string>
#include<stack>
#include<array>
#include<vector>

int get_number_of_stacks(std::ifstream *file) {
    std::string line;
    std::getline(*file,line);
    return (line.length() / 4) + 1;
}

std::vector<char> get_initialised_stack(int stack_num, std::ifstream *file) {
    file->seekg(0, file->beg); // We set the cursor at the beginning.
    std::vector<char> stack;

    while(true) {
        std::string line;
        std::getline(*file, line);

        int position_of_crate = (stack_num*4) + 1;
        char crate_value = line.at(position_of_crate);

        if (crate_value == ('0' + stack_num + 1)) break;
        if (crate_value == ' ') continue;

        stack.insert(stack.begin(), crate_value);
    }

    return stack;
}

void print_stack(std::vector<char> stack) {
    for(auto it=stack.begin(); it!=stack.end(); it++) {
        std::cout<<*it;
    }
    std::cout<<"|"<<std::endl;
}

std::array<int,3> get_crate_move_details(std::string line) {
    std::size_t crate_num_start = 5;
    std::size_t crate_num_end = line.find(" from");

    std::size_t src_stack_start = crate_num_end + 6;
    std::size_t src_stack_end = line.find(" to");

    std::size_t dest_stack_start = src_stack_end + 4;

    return std::array<int,3> {
        // crates_to_move
        std::stoi(line.substr(
            crate_num_start,
            crate_num_end-crate_num_start
        ), nullptr, 10),
        // source_crate
        std::stoi(line.substr(
            src_stack_start,
            src_stack_end-src_stack_start
        ), nullptr, 10),
        // destination_crate
        std::stoi(line.substr(
            dest_stack_start
        ), nullptr, 10)
    };
}

int main() {
    std::ifstream file;
    file.open("day5-input.txt", std::ios::in);

    std::vector<std::vector<char>> stacks;
    int num_of_stacks = get_number_of_stacks(&file);

    for(int i=0; i<num_of_stacks; i++) {
       stacks.push_back(get_initialised_stack(i, &file));
    }

    while(!file.eof()) {
        std::string line;
        std::getline(file, line);

        if (line.length() == 0) continue;

        auto move_details = get_crate_move_details(line);
        int crates_to_move = move_details[0];
        int src_crate_num = move_details[1];
        int dest_crate_num = move_details[2];

        auto src_crate = &stacks.at(src_crate_num-1);
        auto dest_crate = &stacks.at(dest_crate_num-1);

        std::vector<char> crates_to_insert;
        for(int i=src_crate->size()-crates_to_move; i<src_crate->size(); i++) {
            char crate = src_crate->at(i);
            crates_to_insert.push_back(crate);
        }

        dest_crate->insert(dest_crate->end(), crates_to_insert.begin(), crates_to_insert.end());

        for(int i=0; i<crates_to_move; i++) {
            src_crate->pop_back();
        }
    }

    for(auto it=stacks.begin(); it!=stacks.end(); it++) {
        std::cout<<it->back();
    }
    std::cout<<std::endl;

    file.close();
    return 0;
}
