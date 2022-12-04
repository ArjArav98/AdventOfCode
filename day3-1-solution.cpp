#include<iostream>
#include<string>
#include<fstream>

char get_character_in_both_halves_of_string(std::string str) {
    int halfway = str.length()/2;
    std::string::iterator it_start = str.begin();
    std::string::iterator it_end = str.begin() + halfway;

    for (auto it=it_start; it!=it_end; it++) {
        bool char_is_second_half = str.find(*it, halfway) != std::string::npos;
        if (char_is_second_half) return *it;
    }

    return '\0';
}

int get_number_for_character(char character) {
    if (character == '\0') return 0;

    bool is_lowercase_char = character > 'Z';
    return (is_lowercase_char) ? ((character-'a') + 1) : ((character-'A') + 26 + 1);
}

int main() {
    std::ifstream file;
    file.open("day3-1-input.txt", std::ios::in);

    int total = 0;

    while(!file.eof()) {
        std::string line;
        file>>line;

        //std::cout<<line;
        char character = get_character_in_both_halves_of_string(line);
        int number = get_number_for_character(character);
        total += number;
    }

    std::cout<<total<<std::endl;

    return 0;
}
