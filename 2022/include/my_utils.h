#include<iostream>
#include<utility>
#include<vector>
#include<string>

/* -------- */
/* LISTINGS */
/* -------- */

namespace std {
    template <typename A, typename B>
    std::ostream& operator<<(std::ostream& out, std::pair<A,B> pr);
    
    template <typename A>
    std::ostream& operator<<(std::ostream& out, std::vector<A> vec);
}

namespace my_utils {
    std::vector<std::string> split(std::string str, std::string delimiter);
}

/* ----------- */
/* DEFINITIONS */
/* ----------- */

template <typename A, typename B>
std::ostream& std::operator<<(std::ostream& out, std::pair<A,B> pr) {
    return out<<"("<<pr.first<<", "<<pr.second<<")";
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

std::vector<std::string> my_utils::split(std::string str, std::string delimiter) {
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
