#include<iostream>
#include<string>
#include<fstream>
#include<array>
#include<algorithm>

bool sprite_contains_current_pixel (
    int current_pixel, int sprite_middle_pos
) {
    return (
        (current_pixel >= (sprite_middle_pos-1)) &&
        (current_pixel <= (sprite_middle_pos+1))
    );
}

int main() {
    std::ifstream file;
    file.open("input/day10-input.txt", std::ios::in);

    int current_pixel = 0, sprite_middle_pos = 1;
    
    std::array<char, 240> screen;
    int screen_width = 40;

    while(!file.eof()) {
        std::string command;
        file>>command;

        int cycles_to_perform = 0, add_value = 0;

        if (command == "noop") cycles_to_perform = 1;
        else if (command == "addx") {
            file>>add_value;
            cycles_to_perform = 2;
        }

        for(int i=0; i<cycles_to_perform; i++, current_pixel++) {
            bool _sprite_contains_current_pixel = sprite_contains_current_pixel(
                current_pixel % screen_width, sprite_middle_pos % screen_width
            );
            screen[current_pixel] = (_sprite_contains_current_pixel)? '#' : '.';
        }

        sprite_middle_pos += add_value;
    }

    for(int i=0; i<240; i++) {
        if (i % screen_width == 0) std::cout<<std::endl;
        std::cout<<screen[i];
    }

    return 0;
}
