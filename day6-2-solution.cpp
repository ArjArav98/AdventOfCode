#include<iostream>
#include<fstream>
#include<set>

int get_length_of_input(std::ifstream *file) {
    file->seekg(0, file->end);
    int length = file->tellg();
    file->seekg(0, file->beg);
    return length;
}

char get_character(std::ifstream *file) {
    char character = file->get();
    if (character == '\n') return get_character(file);
    return character;
}

int main() {
    std::ifstream file;
    file.open("day6-input.txt", std::ios::in);

    int length_of_input = get_length_of_input(&file);
    int index=0;

    while(!file.eof()) {
        char char1 = get_character(&file);
        int position = file.tellg();
        
        std::set<char> charset;
        charset.insert(char1);

        for(int i=0; i<13; i++) {
            charset.insert(get_character(&file));
        }

        if(file.eof()) break;

        index++;

        if (charset.size() == 14) {
            index+=13;
            break;
        }

        file.seekg(position);
    }

    std::cout<<index<<std::endl;

    file.close();
    return 0;
}
