#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<numeric>

#include"../include/my_utils.h"

class Coord {
    public:
        int x, y, z;
        int non_adjacent_faces;

        Coord(std::string line) {
            auto coord = my_utils::split(line, ",");

            this->x = std::stoi(coord[0], nullptr, 10);
            this->y = std::stoi(coord[1], nullptr, 10);
            this->z = std::stoi(coord[2], nullptr, 10);

            this->non_adjacent_faces = 6;
        }

    bool is_adjacent_to(Coord& coord) {
        return (
            std::abs(this->x - coord.x) +
            std::abs(this->y - coord.y) +
            std::abs(this->z - coord.z)
        ) == 1;
    }

    private:
        friend bool operator<(Coord coord1, Coord coord2);
        friend std::ostream& operator<<(std::ostream& out, Coord& coord);
};

bool operator<(Coord coord1, Coord coord2) {
    bool cond1 = (coord1.x < coord2.x);
    if (cond1) return true;

    bool cond2 = (coord1.x == coord2.x) && (coord1.y < coord2.y);
    if (cond2) return true;

    bool cond3 = (coord1.x == coord2.x) && (coord1.y == coord2.y) && (coord1.z < coord2.z);
    return cond3;
}

std::ostream& operator<<(std::ostream& out, Coord& coord) {
    return out<<"("<<coord.x<<","<<coord.y<<","<<coord.z<<")";
}

int main() {
    std::ifstream file;
    file.open("input/day18-input.txt", std::ios::in);

    std::vector<Coord> coords;

    /* We parse the file. */
    while(!file.eof()) {
        std::string line;
        std::getline(file, line);

        if (file.eof()) break;
        if (line.length() == 0) break;

        coords.push_back(Coord(line));
    }

    std::sort(coords.begin(), coords.end());

    /* We iterate over the coords. */
    for (int coord_index=0; coord_index<coords.size(); coord_index++) {
        for (int comp_index=coord_index+1; comp_index<coords.size(); comp_index++) {
            auto coord = coords.at(coord_index);
            auto comp_coord = coords.at(comp_index);

            /* Since the list is sorted, we know the point after which 
             * it's impossible for adjacent cubes to exist. */
            if (std::abs(coord.x - comp_coord.x) > 1) break;
            if (std::abs(coord.y - comp_coord.y) > 1) continue;
            if (std::abs(coord.z - comp_coord.z) > 1) continue;

            if (coord.is_adjacent_to(comp_coord)) {
                coords.at(coord_index).non_adjacent_faces--;
                coords.at(comp_index).non_adjacent_faces--;
            }
        }
    }

    int area = std::reduce(
        coords.begin(),
        coords.end(),
        0,
        [] (int acc, Coord coord) -> int {
            return acc + coord.non_adjacent_faces;
        }
    );

    std::cout<<area<<std::endl;

    return 0;
}
