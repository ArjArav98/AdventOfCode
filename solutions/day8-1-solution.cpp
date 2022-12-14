#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include<vector>

class Forest {
    public:
        std::vector<std::vector<int>> rows;
        std::vector<std::vector<int>> transposed_rows;

    std::vector<int> column_at(int column_index) {
        std::vector<int> column;
        column.resize(rows[0].size());

        std::transform(
            rows.begin(),
            rows.end(),
            column.begin(),
            [column_index] (std::vector<int> row) -> int { return row.at(column_index); }
        );

        return column;
    }

    void transpose() {
        transposed_rows.clear();
        for (int i=0; i<rows[0].size(); i++)
            transposed_rows.push_back(column_at(i));
    }

    bool tree_visible_at(int row_index, int col_index) {
        if (col_index == 0 || col_index == (rows[0].size()-1)) return true;
        if (row_index == 0 || row_index == (rows.size()-1)) return true;

        auto row = rows.at(row_index);
        auto transposed_column = transposed_rows.at(col_index);
        auto this_tree_size = row.at(col_index);

        auto tallest_to_left = std::all_of(
            row.begin(),
            row.begin() + col_index,
            [this_tree_size] (int tree_size) -> bool { return tree_size < this_tree_size; }
        );
        if (tallest_to_left) return true;

        auto tallest_to_right = std::all_of(
            row.begin() + col_index + 1,
            row.end(),
            [this_tree_size] (int tree_size) -> bool { return tree_size < this_tree_size; }
        );
        if (tallest_to_right) return true; 

        auto tallest_to_top = std::all_of(
            transposed_column.begin(),
            transposed_column.begin() + row_index,
            [this_tree_size] (int tree_size) -> bool { return tree_size < this_tree_size; }
        );
        if (tallest_to_top) return true; 
        
        auto tallest_to_bottom = std::all_of(
            transposed_column.begin() + row_index + 1,
            transposed_column.end(),
            [this_tree_size] (int tree_size) -> bool { return tree_size < this_tree_size; }
        );
        if (tallest_to_bottom) return true;

        return false;
    }
};

int main() {
    std::ifstream file;
    file.open("input/day8-input.txt", std::ios::in);

    Forest forest;

    while(!file.eof()) {
        std::string line;
        file>>line;
    
        if (line.size() == 0) continue;

        std::vector<int> forest_row(line.size(), 0);

        std::transform(
            line.begin(),
            line.end(),
            forest_row.begin(),
            [] (char tree_size) -> int { return tree_size-'0'; }
        );

        forest.rows.push_back(forest_row);
    }

    forest.transpose();
    int visible_trees = 0;

    for (int row_index=0; row_index<forest.rows.size(); row_index++) {
        auto row = forest.rows.at(row_index);
        for (int col_index=0; col_index<row.size(); col_index++) {
            if (forest.tree_visible_at(row_index,col_index)) visible_trees++;
        }
    }
    std::cout<<visible_trees<<std::endl;

    return 0;
}
