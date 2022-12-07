#include<iostream>
#include<string>
#include<fstream>

char get_common_character_in_all_strings(std::string str1, std::string str2, std::string str3) {
    for (auto it=str1.begin(); it!=str1.end(); it++) {
        bool in_second_string = str2.find(*it, 0) != std::string::npos;
        bool in_third_string = str3.find(*it, 0) != std::string::npos;
        
        if (in_second_string && in_third_string) return *it;
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
    file.open("input/day3-1-input.txt", std::ios::in);

    int total = 0;

    while(!file.eof()) {
        std::string str1, str2, str3;
        file>>str1>>str2>>str3;
        
        char character = get_common_character_in_all_strings(
            str1, str2, str3
        );
        int number = get_number_for_character(character);
        total += number;
    }

    std::cout<<total<<std::endl;

    return 0;
}
