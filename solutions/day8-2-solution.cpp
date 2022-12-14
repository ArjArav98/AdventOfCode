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

    int get_scenic_score_at(int row_index, int col_index) {
        if (col_index == 0 || col_index == (rows[0].size()-1)) return 0;
        if (row_index == 0 || row_index == (rows.size()-1)) return 0;

        auto row = rows.at(row_index);
        auto transposed_column = transposed_rows.at(col_index);
        int curr_tree_size = row.at(col_index);
        int scenic_score = 1; 

        auto compare_tree_size = 
            [curr_tree_size] (int tree_size) -> bool {
                return tree_size >= curr_tree_size;
            };

        auto find_tallest_in_range =
            [curr_tree_size, compare_tree_size] (auto start_it, auto end_it) {
                return std::find_if(start_it, end_it, compare_tree_size);
            };

        auto rev_curr_elem_iterator = row.rbegin() + (row.size() - col_index - 1);
        auto leftmost_iterator = row.rend();
        auto tallest_to_left = find_tallest_in_range(rev_curr_elem_iterator+1, leftmost_iterator); 
        auto distance = std::distance(rev_curr_elem_iterator, tallest_to_left);
        scenic_score *= (tallest_to_left == leftmost_iterator)? distance-1 : distance;

        auto current_elem_iterator = row.begin() + col_index;
        auto rightmost_iterator = row.end();
        auto tallest_to_right = find_tallest_in_range(current_elem_iterator+1, rightmost_iterator); 
        distance = std::distance(current_elem_iterator, tallest_to_right);
        scenic_score *= (tallest_to_right == rightmost_iterator)? distance-1 : distance;
        
        rev_curr_elem_iterator = transposed_column.rbegin() + (transposed_column.size() - row_index - 1);
        auto topmost_iterator = transposed_column.rend();
        auto tallest_to_top = find_tallest_in_range(rev_curr_elem_iterator+1, topmost_iterator); 
        distance = std::distance(rev_curr_elem_iterator, tallest_to_top);
        scenic_score *= (tallest_to_top == topmost_iterator)? distance-1 : distance;
       
        current_elem_iterator = transposed_column.begin() + row_index;
        auto bottommost_iterator = transposed_column.end();
        auto tallest_to_bottom = find_tallest_in_range(current_elem_iterator+1, bottommost_iterator); 
        distance = std::distance(current_elem_iterator, tallest_to_bottom);
        scenic_score *= (tallest_to_bottom == bottommost_iterator)? distance-1 : distance;

        return scenic_score;
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
    int max_scenic_score = 0;

    for (int row_index=0; row_index<forest.rows.size(); row_index++) {
        auto row = forest.rows.at(row_index);
        for (int col_index=0; col_index<row.size(); col_index++) {
            int scenic_score = forest.get_scenic_score_at(row_index,col_index);
            if (scenic_score > max_scenic_score) max_scenic_score = scenic_score;
        }
    }
    std::cout<<max_scenic_score<<std::endl;

    return 0;
}
