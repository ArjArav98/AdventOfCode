namespace std {
    template <typename A>
    std::ostream& operator<<(std::ostream& out, std::vector<A> vec);
}

namespace aoc {
    class GameSubset;
    class Game;

    std::ostream& operator<<(std::ostream& output, const GameSubset& subset);
    std::ostream& operator<<(std::ostream& output, const Game& game);

    std::vector<std::string> split(std::string str, std::string delimiter);

    Game get_game_data_from_line(std::string line);
};

/* ----------------- */
/* -- DEFINITIONS -- */
/* ----------------- */

class aoc::GameSubset {
  public:
    int red_cubes;
    int blue_cubes;
    int green_cubes;

    GameSubset(std::string data_line) {
        red_cubes = blue_cubes = green_cubes = 0;

        auto line_splits = aoc::split(data_line, ",");
        for (auto line_split: line_splits) {
            auto cube_split = aoc::split(line_split, " ");
            
        }
    }

    GameSubset(int red_cubes, int blue_cubes, int green_cubes) {
        this->red_cubes=red_cubes;
        this->blue_cubes=blue_cubes;
        this->green_cubes=green_cubes;
    }

    bool operator==(const GameSubset& subset) const {
        return (
            this->red_cubes == subset.red_cubes &&
            this->blue_cubes == subset.blue_cubes &&
            this->green_cubes == subset.green_cubes
        );
    }

    friend std::ostream& operator<<(std::ostream& output, const GameSubset& subset);
};

class aoc::Game {
  public:
    int id;
    std::vector<GameSubset> subsets;

    Game(int id, std::vector<GameSubset> subsets) {
        this->id=id;
        this->subsets.assign(subsets.begin(), subsets.end());
    }

    bool operator==(const Game& game) const {
        return (this->id == game.id) && (this->subsets == game.subsets);
    }

    friend std::ostream& operator<<(std::ostream& output, const Game& game);
};

std::ostream& aoc::operator<<(std::ostream& output, const GameSubset& subset) {
    output << "{ red: " << subset.red_cubes << ", blue: " << subset.blue_cubes;
    output << ", green: " << subset.green_cubes << " }";
    return output;
}

std::ostream& aoc::operator<<(std::ostream& output, const Game& game) {
    output << "{ id: " << game.id << ", subsets: " << game.subsets << " }";
    return output;
}

template <typename A>
std::ostream& std::operator<<(std::ostream& out, std::vector<A> vec) {
    out<<"[";
    for (auto it=vec.begin(); it != vec.end(); it++) {
        if (it == (vec.end() - 1)) out<<*it;
        else out<<*it<<", ";
    }
    return out<<"]";
}

std::vector<std::string> aoc::split(std::string str, std::string delimiter) {
    std::vector<std::string> splits;
    auto delimiter_pos = str.find(delimiter);

    if (delimiter_pos == std::string::npos) {
        splits.insert(splits.begin(), str);
        return splits;
    }

    auto rest_of_splits = split(str.substr(delimiter_pos + delimiter.length()), delimiter);
    splits.insert(splits.begin(), rest_of_splits.begin(), rest_of_splits.end());
    splits.insert(splits.begin(), str.substr(0, delimiter_pos));
    
    return splits;
}

aoc::Game aoc::get_game_data_from_line(std::string line) {
    // Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green

    auto line_split = aoc::split(line, ":");
    std::string game_metadata = line_split.at(0), game_data = line_split.at(1);

    int game_id = std::stoi(aoc::split(game_metadata, " ")[1]);

    std::vector<std::string> subset_strings = aoc::split(game_data, ";");
    std::vector<aoc::GameSubset> subsets;

    std::transform(
        subset_strings.begin(), subset_strings.end(), subsets.begin(),
        [](std::string subset_string) -> aoc::GameSubset {
            return aoc::GameSubset(subset_string);
        }
    );

    return aoc::Game(-1, std::vector<GameSubset>{ aoc::GameSubset(0,0,0) });
}
