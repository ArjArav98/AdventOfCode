#include<iostream>
#include<fstream>
#include<set>
#include<array>
#include<utility>

enum Direction {
    Top,
    Bottom,
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

        RopeSegment *next_segment;

    public:
        RopeSegment(int segment_number) {
            head_coords = std::make_pair(0,0);
            tail_coords = std::make_pair(0,0);

            visited_coords.insert(tail_coords);

            if (segment_number == 8) next_segment = nullptr;
            else next_segment = new RopeSegment(segment_number+1);
        }

        ~RopeSegment() {
            delete next_segment;
        }

        void shift_head(Direction direction) {
            switch (direction) {
                case Direction::Top:
                    head_coords.second += 1; break;
                case Direction::Bottom:
                    head_coords.second -= 1; break;
                case Direction::Left:
                    head_coords.first -= 1; break;
                case Direction::Right:
                    head_coords.first += 1; break;
                case Direction::TopRight:
                    head_coords.first += 1;
                    head_coords.second += 1;
                    break;
                case Direction::BottomRight:
                    head_coords.first += 1;
                    head_coords.second -= 1;
                    break;
                case Direction::BottomLeft:
                    head_coords.first -= 1;
                    head_coords.second -= 1;
                    break;
                case Direction::TopLeft:
                    head_coords.first -= 1;
                    head_coords.second += 1;
                    break;
                default: break;
            }
        }

        void shift_tail(Direction direction) {
            switch (direction) {
                case Direction::Top:
                    tail_coords.second += 1; break;
                case Direction::Bottom:
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
                auto lin_direction = determine_linear_direction_of_head();
                shift_tail(lin_direction);
                if (next_segment != nullptr) next_segment->move_head(lin_direction);
            }
            else if (tail_not_touching_head_diagonally()) {
                auto diag_direction = determine_diagonal_direction_of_head();
                shift_tail(diag_direction);
                if (next_segment != nullptr) next_segment->move_head(diag_direction);
            }

            // We only record the visited_coords for the last segment.
            if (next_segment == nullptr) visited_coords.insert(tail_coords);
        }

        Direction determine_linear_direction_of_head() {
            int x_diff = head_coords.first - tail_coords.first;
            int y_diff = head_coords.second - tail_coords.second;

            if ((x_diff == 0) && (y_diff > 0)) return Direction::Top;
            else if ((x_diff == 0) && (y_diff < 0)) return Direction::Bottom;
            else if ((y_diff == 0) && (x_diff > 0)) return Direction::Right;
            else if ((y_diff == 0) && (x_diff < 0)) return Direction::Left;

            return Direction::None;
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
            if (next_segment == nullptr) return visited_coords.size();
            else return next_segment->get_num_visited_coords();
        }
};

Direction get_direction(char direction) {
    switch (direction) {
        case 'U': return Direction::Top;
        case 'D': return Direction::Bottom;
        case 'R': return Direction::Right;
        case 'L': return Direction::Left;
    }

    return Direction::None;
}

int main() {
    std::ifstream file;
    file.open("input/day9-input.txt", std::ios::in);

    RopeSegment rope_segment(0);

    while(!file.eof()) {
        char character;
        int move_num;
        file>>character>>move_num;

        if (file.eof()) continue;

        auto direction = get_direction(character);
        for (int i=0; i<move_num; i++) rope_segment.move_head(direction);
    }

    std::cout<<rope_segment.get_num_visited_coords()<<std::endl;

    return 0;
}
