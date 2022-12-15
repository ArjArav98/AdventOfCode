#include<iostream>
#include<fstream>
#include<set>
#include<array>
#include<utility>

enum Direction {
    Up,
    Down,
    Right,
    Left,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
    None
};

class RopeSegment {
    private:
        typedef std::pair<int,int> Coords;

        Coords head_coords;
        Coords tail_coords;
        std::set<Coords> visited_coords;

    public:
        RopeSegment() {
            head_coords = std::make_pair(0,0);
            tail_coords = std::make_pair(0,0);

            visited_coords.insert(tail_coords);
        }

        void shift_head(Direction direction) {
            switch (direction) {
                case Direction::Up:
                    head_coords.second += 1; break;
                case Direction::Down:
                    head_coords.second -= 1; break;
                case Direction::Right:
                    head_coords.first += 1; break;
                case Direction::Left:
                    head_coords.first -= 1; break;
                default: break;
            }
        }

        void shift_tail(Direction direction) {
            switch (direction) {
                case Direction::Up:
                    tail_coords.second += 1; break;
                case Direction::Down:
                    tail_coords.second -= 1; break;
                case Direction::Left:
                    tail_coords.first -= 1; break;
                case Direction::Right:
                    tail_coords.first += 1; break;
                case Direction::TopRight:
                    tail_coords.first += 1;
                    tail_coords.second += 1;
                    break;
                case Direction::BottomRight:
                    tail_coords.first += 1;
                    tail_coords.second -= 1;
                    break;
                case Direction::BottomLeft:
                    tail_coords.first -= 1;
                    tail_coords.second -= 1;
                    break;
                case Direction::TopLeft:
                    tail_coords.first -= 1;
                    tail_coords.second += 1;
                    break;
                default: break;
            }
        }

        void move_head(Direction direction) {
            shift_head(direction);

            if (tail_right_after_head() || tail_at_head());
            else if (tail_on_same_line_as_head()) {
                shift_tail(direction);
            }
            else if (tail_not_touching_head_diagonally()) {
                auto diag_direction = determine_diagonal_direction_of_head();
                shift_tail(diag_direction);
            }

            visited_coords.insert(tail_coords);
        }

        Direction determine_diagonal_direction_of_head() {
            int x_diff = head_coords.first - tail_coords.first;
            int y_diff = head_coords.second - tail_coords.second;

            if ((x_diff > 0) && (y_diff > 0)) return Direction::TopRight;
            else if ((x_diff > 0) && (y_diff < 0)) return Direction::BottomRight;
            else if ((x_diff < 0) && (y_diff < 0)) return Direction::BottomLeft;
            else if ((x_diff < 0) && (y_diff > 0)) return Direction::TopLeft;
        
            return Direction::None;
        }

        bool tail_right_after_head() {
            return (
                std::abs(head_coords.first - tail_coords.first) +
                std::abs(head_coords.second - tail_coords.second)
            ) == 1;
        }

        bool tail_on_same_line_as_head() {
            return (
                ((head_coords.first - tail_coords.first) == 0) ||
                ((head_coords.second - tail_coords.second) == 0)
            );
        }

        bool tail_not_touching_head_diagonally() {
            std::array<Coords,4> diagonal_positions = {
                Coords(tail_coords.first+1, tail_coords.second+1),
                Coords(tail_coords.first+1, tail_coords.second-1),
                Coords(tail_coords.first-1, tail_coords.second-1),
                Coords(tail_coords.first-1, tail_coords.second+1)
            };

            return !std::any_of(
                diagonal_positions.begin(),
                diagonal_positions.end(),
                [this] (Coords coords) -> bool { return this->head_coords == coords; }
            );
        }

        bool tail_at_head() {
            return head_coords == tail_coords;
        }

        int get_num_visited_coords () {
            return visited_coords.size();
        }
};

Direction get_direction(char direction) {
    switch (direction) {
        case 'U': return Direction::Up;
        case 'D': return Direction::Down;
        case 'R': return Direction::Right;
        case 'L': return Direction::Left;
    }

    return Direction::None;
}

int main() {
    std::ifstream file;
    file.open("input/day9-input.txt", std::ios::in);

    RopeSegment rope;

    while(!file.eof()) {
        char character;
        int move_num;
        file>>character>>move_num;

        if (file.eof()) continue;

        auto direction = get_direction(character);

        for (int i=0; i<move_num; i++) rope.move_head(direction);
    }

    std::cout<<rope.get_num_visited_coords()<<std::endl;

    return 0;
}
