/* Copyright 2023 Arjun Aravind */
#ifndef INCLUDE_AOC_H_ // NOLINT
#define INCLUDE_AOC_H_

#include<map>
#include<set>
#include<string>
#include<vector>

namespace std {

template <typename A>
std::ostream& operator<<(std::ostream& out, std::vector<A> vec);

template <typename A>
std::ostream& operator<<(std::ostream& out, std::set<A> set);

template <typename A, typename B>
std::ostream& operator<<(std::ostream& out, std::map<A, B> map);

};  // namespace std

namespace aoc {

class Coord;

std::vector<std::string> split(std::string str, std::string delimiter);
std::ostream& operator<<(std::ostream& out, const Coord coord);

};  // namespace aoc

/* ----------------- */
/* -- DEFINITIONS -- */
/* ----------------- */

template <typename A>
std::ostream& std::operator<<(std::ostream& out, std::vector<A> vec) {
    out << "[";
    for (auto element : vec) out << element << ", ";
    return out << "\b]";
}

template <typename A>
std::ostream& std::operator<<(std::ostream& out, std::set<A> set) {
    out << "{";
    for (auto element : set) out << element << ", ";
    return out << "\b}";
}

template <typename A, typename B>
std::ostream& std::operator<<(std::ostream& out, std::map<A, B> map) {
    out << "{";
    for (auto element : map)
        out << element.first << ": " << element.second << ", ";
    return out << "\b}";
}

std::vector<std::string> aoc::split(std::string str, std::string delimiter) {
    std::vector<std::string> splits;
    auto delimiter_pos = str.find(delimiter);

    if (delimiter_pos == std::string::npos) {
        splits.insert(splits.begin(), str);
        return splits;
    }

    auto rest_of_splits = split(
        str.substr(delimiter_pos + delimiter.length()), delimiter);
    splits.insert(splits.begin(), rest_of_splits.begin(), rest_of_splits.end());
    splits.insert(splits.begin(), str.substr(0, delimiter_pos));

    return splits;
}

class aoc::Coord {
  public: // NOLINT
    int row, col;

    Coord(int row, int col) {
        this->row = row;
        this->col = col;
    }

    bool operator==(const Coord coord) {
        return (
            (this->row == coord.row) &&
            (this->col == coord.col));
    }

    friend std::ostream& operator<<(std::ostream& out, const Coord coord);
};

std::ostream& aoc::operator<<(std::ostream& out, const aoc::Coord coord) {
    return out << "{" << coord.row << ", " << coord.col << "}";
}

#endif  // INCLUDE_AOC_H_ // NOLINT
