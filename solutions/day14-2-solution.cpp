#include<iostream>
#include<string>
#include<fstream>
#include<utility>
#include<functional>
#include<set>
#include<vector>
#include<algorithm>
#include<array>

#include"../include/my_utils.h"

/* ------- */
/* CLASSES */
/* ------- */

class LineSegment {
    public:
        std::pair<int,int> starting;
        std::pair<int,int> ending;

        LineSegment(std::pair<int,int> starting, std::pair<int,int> ending) {
            this->starting = starting;
            this->ending = ending;
        }
   
    bool is_horizontal() {
        return (starting.second - ending.second) == 0;
    }

    bool blocks_falling_sand_vertically(std::pair<int,int> falling_sand_coord) {
        if (is_horizontal()) return false;
    
        auto result = (falling_sand_coord.first == starting.first) && (
            (
                (falling_sand_coord.second >= starting.second) &&
                (falling_sand_coord.second <= ending.second)
            ) ||
            (
                (falling_sand_coord.second >= ending.second) &&
                (falling_sand_coord.second <= starting.second)
            )
        );

        return result;
    }

    bool blocks_falling_sand_horizontally(std::pair<int,int> falling_sand_coord) {
        if (!is_horizontal()) return false;

        auto result = (falling_sand_coord.second == starting.second) && (
            (
                (falling_sand_coord.first >= starting.first) &&
                (falling_sand_coord.first <= ending.first)
            ) ||
            (
                (falling_sand_coord.first >= ending.first) &&
                (falling_sand_coord.first <= starting.first)
            )
        );

        return result;
    }

    bool operator==(const LineSegment ls2) {
        return (
            ((this->starting == ls2.starting) && (this->ending == ls2.ending)) ||
            ((this->starting == ls2.ending) && (this->ending == ls2.starting))
        );
    }

    private:
        friend std::ostream& operator<<(std::ostream& out, LineSegment line);
};

std::ostream& operator<<(std::ostream& out, LineSegment line) {
    return out<<line.starting<<" -> "<<line.ending;
}

/* ------- */
/* UTILITY */
/* ------- */

std::pair<int,int> string_coord_to_pair(std::string coord) {
    auto coord_comma_pos = coord.find(",");
    return std::make_pair(
        std::stoi(coord.substr(0, coord_comma_pos), nullptr, 10),
        std::stoi(coord.substr(coord_comma_pos+1), nullptr, 10)
    );
}

int largest_y_coord_in_segments(
    std::vector<LineSegment> segments
) {
    int max = 0;

    std::for_each(
        segments.begin(),
        segments.end(),
        [&max] (LineSegment segment) -> void {
            max = (segment.starting.second > max)? segment.starting.second : max;
            max = (segment.ending.second > max)? segment.ending.second : max;
        }
    );

    return max;
}

bool falling_sand_particle_blocked_by_segment(
    std::vector<LineSegment>& rock_line_segments,
    std::pair<int,int> falling_sand_coord
) {
    return std::any_of(
        rock_line_segments.begin(),
        rock_line_segments.end(),
        [falling_sand_coord] (LineSegment segment) -> bool {
            auto blocked_vertically = (
                segment.blocks_falling_sand_vertically(falling_sand_coord)
            );
            if (blocked_vertically) return true;

            auto blocked_horizontally = (
                segment.blocks_falling_sand_horizontally(falling_sand_coord)
            );

            return blocked_horizontally;
        }
    );
}

bool falling_sand_particle_blocked_by_sand(
    std::set<std::pair<int,int>> sand_particle_coords,
    std::pair<int,int> falling_sand_coord
) {
    auto element = sand_particle_coords.find(falling_sand_coord);
    return element != sand_particle_coords.end();
}

bool sand_particle_stop_at_sand_source(
    std::vector<LineSegment>& rock_line_segments,
    std::set<std::pair<int,int>>& sand_particle_coords,
    int floor_y_coord 
) {
    /* Assumption: Origin point is in top-left. */
    /* 500 is the x-axis and 0 is the y-axis. */ 
    auto sand_starting_point = std::make_pair(500,0);
    auto falling_sand_coord = sand_starting_point;
    bool sand_is_falling = true;

    std::array<std::pair<int,int>,3> coord_ops {
        std::make_pair(0,1), // bottom
        std::make_pair(-1,1), // left bottom diagonal
        std::make_pair(1,1), // right bottom diagonal
    };

    while(sand_is_falling) {
        /* We iterate over the coord_ops to see which one to use. */
        auto successful_coord_op = std::find_if(
            coord_ops.begin(),
            coord_ops.end(),
            [&] (auto coord_op) -> bool {
                auto coord = std::make_pair(
                    falling_sand_coord.first + coord_op.first,
                    falling_sand_coord.second + coord_op.second
                );

                auto blocked_by_sand =
                    falling_sand_particle_blocked_by_sand(
                        sand_particle_coords, coord 
                    );
                if (blocked_by_sand) return false;

                auto blocked_by_segment =
                    falling_sand_particle_blocked_by_segment(
                        rock_line_segments, coord 
                    );
                if (blocked_by_segment) return false;

                auto blocked_by_floor = coord.second == floor_y_coord;

                std::cout<<"Falling at "<<coord<<" ";
                std::cout<<"Sand - "<<blocked_by_sand<<" ";
                std::cout<<"Segment - "<<blocked_by_segment<<std::endl;

                return !blocked_by_floor;
            }
        );

        if (successful_coord_op == coord_ops.end()) {
            if (falling_sand_coord == sand_starting_point) return true;
            break;
        }

        falling_sand_coord.first += successful_coord_op->first;
        falling_sand_coord.second += successful_coord_op->second;
    }

    sand_particle_coords.insert(falling_sand_coord);
    std::cout<<"The particle is at rest at "<<falling_sand_coord<<std::endl<<std::endl;

    return false;
}

/* ---- */
/* MAIN */
/* ---- */

int main() {
    std::ifstream file;
    file.open("input/day14-input.txt", std::ios::in);

    std::vector<LineSegment> rock_line_segments;
    std::set<std::pair<int,int>> sand_particle_coords;

    /* We parse the segments from the file input. */
    while(!file.eof()) {
        std::string line;
        std::getline(file, line);
        
        if (file.eof()) break;

        auto splits = my_utils::split(line, " -> ");

        for (int i=0; i<splits.size()-1; i++) {
            auto segment = LineSegment(
                string_coord_to_pair(splits[i]),
                string_coord_to_pair(splits[i+1])
            );
            auto exists = std::any_of(
                rock_line_segments.begin(),
                rock_line_segments.end(),
                [&segment] (LineSegment seg) -> bool {
                    return segment == seg;
                }
            );

            if (!exists) rock_line_segments.push_back(segment);
        }
    }

    auto floor_y_coord = largest_y_coord_in_segments(rock_line_segments) + 2;
    int count = 0;

    while(true) {
        bool particle_stopped_at_sand_source = sand_particle_stop_at_sand_source(
            rock_line_segments,
            sand_particle_coords,
            floor_y_coord
        );

        count++;
        if (particle_stopped_at_sand_source) break;
    }

    std::cout<<count<<std::endl;

    return 0;
}
