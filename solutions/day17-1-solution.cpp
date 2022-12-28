#include<iostream>
#include<vector>
#include<array>
#include<utility>
#include<fstream>
#include<string>
#include<numeric>
#include<algorithm>

#include"../include/my_utils.h"

/* ------- */
/* CLASSES */
/* ------- */

typedef std::pair<int,int> Coord;

class RockSegment {
    public:
        Coord starting;
        Coord ending;
        bool is_horizontal;
        int length;

        RockSegment(Coord starting, Coord ending) {
            this->is_horizontal = (starting.second - ending.second) == 0;

            auto coords_in_order =
                (is_horizontal)?
                    (starting.first < ending.first) :
                    (starting.second < ending.second);

            this->starting = (coords_in_order)? starting : ending;
            this->ending = (coords_in_order)? ending : starting;

            this->length =
                (is_horizontal)?
                    (ending.first-starting.first)+1 :
                    (ending.second-starting.second)+1;
        }

        bool contains_coord(Coord coord) {
            return (
                (this->is_horizontal) &&
                (coord.second == starting.second) &&
                (coord.first >= starting.first) && (coord.first <= ending.first)
            ) ||
            (
                (!this->is_horizontal) &&
                (coord.first == starting.first) &&
                (coord.second >= starting.second) && (coord.second <= ending.second)
            );
        }

        bool blocks_segment(RockSegment segment) {
            if (this->is_horizontal) {
                for (int i=starting.first; i<=ending.first; i++) {
                    auto coord = std::make_pair(i, starting.second);
                    if (segment.contains_coord(coord)) return true;
                }
            }
            else {
                for (int i=starting.second; i<=ending.second; i++) {
                    auto coord = std::make_pair(starting.first, i);
                    if (segment.contains_coord(coord)) return true;
                }
            }

            return false;
        }

        bool blocks_segments(std::vector<RockSegment> segments) {
            return std::any_of(
                segments.begin(), segments.end(),
                [this] (RockSegment segment) -> bool {
                    return this->blocks_segment(segment);
                }
            );
        }

        bool blocked_by_wall() {
            return (starting.first < 0) || (ending.first > 6);
        }

        bool blocked_by_floor() {
            return (ending.second > 5000);
        }

        private:
            friend std::ostream& operator<<(std::ostream& out, RockSegment& segment);
};

std::ostream& operator<<(std::ostream& out, RockSegment& segment) {
    out<<"RockSegment(starting="<<segment.starting;
    return out<<",ending="<<segment.ending<<")";
}

/* -------------------- */
/* GENERATION FUNCTIONS */
/* -------------------- */

typedef std::vector<RockSegment> Rock;

std::array<Rock,5> rocks {
    Rock { RockSegment(std::make_pair(0,0), std::make_pair(3,0)) },
    Rock {
        RockSegment(std::make_pair(0,1), std::make_pair(2,1)),
        RockSegment(std::make_pair(1,0), std::make_pair(1,2))
    },
    Rock {
        RockSegment(std::make_pair(0,2), std::make_pair(2,2)),
        RockSegment(std::make_pair(2,0), std::make_pair(2,2))
    },
    Rock { RockSegment(std::make_pair(0,0), std::make_pair(0,3)) },
    Rock {
        RockSegment(std::make_pair(0,0), std::make_pair(0,1)),
        RockSegment(std::make_pair(1,0), std::make_pair(1,1))
    }
};

auto _rock_generator() {
    int counter = -1;
    auto nested_func = [counter] () mutable -> Rock {
        return rocks[(++counter)%5];
    };
    return nested_func;
}
auto rock_generator = _rock_generator();

/* ---------- */

auto _jet_direction_generator() {
    std::ifstream file;
    std::string line;
    int counter = -1;

    file.open("input/day17-input.txt", std::ios::in);
    std::getline(file, line);
    file.close();

    auto nested_func = [counter, line] () mutable -> char {
        if (counter == line.length()-1) counter = -1;
        return line[++counter];
    };

    return nested_func;
}
auto jet_direction_generator = _jet_direction_generator();

std::pair<int,int> get_direction_ops() {
    char direction = jet_direction_generator();
    return (direction == '>')? std::make_pair(1,0) : std::make_pair(-1,0);
}

/* --------- */
/* FUNCTIONS */
/* --------- */

int get_tallest_rock_level(std::vector<RockSegment>& segments) {
    /* This means we should get the lowest y coordinate. */
    return std::min_element(
        segments.begin(), segments.end(),
        [] (RockSegment seg1, RockSegment seg2) -> bool {
            return seg1.starting.second < seg2.starting.second;
        }
    )->starting.second;
}

int get_rock_height(Rock& rock) {
    auto elem = std::max_element(
        rock.begin(),
        rock.end(),
        [] (RockSegment& seg1, RockSegment& seg2) -> bool {
            auto seg1_height = (seg1.is_horizontal)? 1 : seg1.length;
            auto seg2_height = (seg2.is_horizontal)? 1 : seg2.length;
            return seg1_height < seg2_height;
        }
    );
    return (elem->is_horizontal)? 1 : elem->length;
}

Coord get_anti_ops(Coord ops) {
    return std::make_pair(-ops.first, -ops.second);
}

void apply_ops_to_rock(Rock& rock, Coord ops) {
    std::for_each(
        rock.begin(), rock.end(),
        [&ops] (RockSegment& segment) -> void {
            segment.starting.first += ops.first;
            segment.starting.second += ops.second;
            segment.ending.first += ops.first;
            segment.ending.second += ops.second;
        }
    );
}

int main() {
    std::vector<RockSegment> segments;

    int floor = 5001;
    int tallest_rock_level = floor;

    for (int i=0; i<2022; i++) {
        auto rock = rock_generator();
        auto origin = std::make_pair(2, tallest_rock_level - 3 - get_rock_height(rock));
        apply_ops_to_rock(rock, origin);

        bool shifting_sideways = true;

        for (; true; shifting_sideways = !shifting_sideways) {
            auto direction =
                (shifting_sideways)?
                    get_direction_ops() : std::make_pair(0,1);
            apply_ops_to_rock(rock, direction);

            auto blocked_by_wall = std::any_of(
                rock.begin(), rock.end(),
                [] (RockSegment& segment) -> bool {
                    return segment.blocked_by_wall();
                }
            );
            if (blocked_by_wall) {
                apply_ops_to_rock(rock, get_anti_ops(direction));
                continue;
            }

            auto blocked_by_segment = std::any_of(
                segments.begin(), segments.end(),
                [&rock] (RockSegment& segment) -> bool {
                    return segment.blocks_segments(rock);
                }
            );
            if (blocked_by_segment) {
                if (shifting_sideways) {
                    apply_ops_to_rock(rock, get_anti_ops(direction));
                    continue;
                }
                else {
                    apply_ops_to_rock(rock, get_anti_ops(direction));
                    int num = get_tallest_rock_level(rock);
                    tallest_rock_level = (num < tallest_rock_level)? num : tallest_rock_level;
                    segments.insert(segments.begin(), rock.begin(), rock.end());
                    break;
                }
                break;
            }

            auto blocked_by_floor = std::any_of(
                rock.begin(), rock.end(),
                [] (RockSegment& segment) -> bool {
                    return segment.blocked_by_floor();
                }
            );
            if (blocked_by_floor) {
                apply_ops_to_rock(rock, get_anti_ops(direction));
                int num = get_tallest_rock_level(rock);
                tallest_rock_level = (num < tallest_rock_level)? num : tallest_rock_level;
                segments.insert(segments.begin(), rock.begin(), rock.end());
                break;
            }
        }

        std::cout<<"The tallest level now is "<<tallest_rock_level<<std::endl;
    }

    std::cout<<floor - tallest_rock_level<<std::endl;
    
    return 0;
}
