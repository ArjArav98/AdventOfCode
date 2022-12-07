#include<iostream>
#include<fstream>
#include<map>
#include<array>

char calculate_player2_sign (char player1_sign, char round_result) {
    if (round_result == 'Y') return player1_sign; 

    if (round_result == 'X') { // player2 has to lose
        std::map<char, char> sign_which_defeats_player2_sign {
            {'B', 'A'},
            {'C', 'B'},
            {'A', 'C'}
        };

        return sign_which_defeats_player2_sign.at(player1_sign);
    }

    // player2 has to win
    std::map<char, char> sign_defeated_by_player2_sign {
        {'A', 'B'},
        {'B', 'C'},
        {'C', 'A'}
    };
    

    return sign_defeated_by_player2_sign.at(player1_sign);
}

int calculate_round_score (char player2_sign, char result) {
    int sign_score, result_score;

    switch(player2_sign) {
        case 'A': sign_score = 1; break;
        case 'B': sign_score = 2; break;
        case 'C': sign_score = 3; break;
        default: sign_score = 0;
    }

    switch(result) {
        case 'Z': result_score = 6; break;
        case 'X': result_score = 0; break;
        case 'Y': result_score = 3; break;
    }

    return sign_score + result_score;
}

int main() {
    
    std::ifstream file;
    file.open("day2-1-input.txt", std::ios::in);

    long total_score = 0;
    long last_score = 0;

    while(!file.eof()) {
        char player1_sign, round_result;
        file>>player1_sign>>round_result;

        last_score = calculate_round_score(
            calculate_player2_sign(player1_sign, round_result),
            round_result
        );
        total_score += last_score;
    }

    // we have to subtract last_score because the last line
    // repeats itself when reading file
    std::cout<<total_score-last_score<<std::endl;

    return 0;
}
