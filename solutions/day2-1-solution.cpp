#include<iostream>
#include<fstream>
#include<map>
#include<array>

enum BattleResult { win, lose, draw };

BattleResult player2_battle_result (char player1_sign, char player2_sign) {
    if (('C' - player1_sign) == ('Z' - player2_sign)) return BattleResult::draw;

    std::map<char, std::array<char,2>> sign_weaknesses {
        {'X', {'B', 'Y'}},
        {'Y', {'C', 'Z'}},
        {'Z', {'A', 'X'}}
    };

    auto weaknesses = sign_weaknesses.at(player2_sign);

    bool player2_weak_against_player1 = std::find(
        weaknesses.begin(),
        weaknesses.end(),
        player1_sign
    ) != weaknesses.end();

    return player2_weak_against_player1 ? BattleResult::lose : BattleResult::win;
}

int calculate_round_score (char player2_sign, BattleResult result) {
    int sign_score, result_score;

    switch(player2_sign) {
        case 'X': sign_score = 1; break;
        case 'Y': sign_score = 2; break;
        case 'Z': sign_score = 3; break;
        default: sign_score = 0;
    }

    switch(result) {
        case BattleResult::win: result_score = 6; break;
        case BattleResult::lose: result_score = 0; break;
        case BattleResult::draw: result_score = 3; break;
    }

    return sign_score + result_score;
}

int main() {
    
    std::ifstream file;
    file.open("input/day2-1-input.txt", std::ios::in);

    long total_score = 0;
    long last_score = 0;

    while(!file.eof()) {
        char player1_sign, player2_sign;
        file>>player1_sign>>player2_sign;

        last_score = calculate_round_score(
            player2_sign,
            player2_battle_result(player1_sign, player2_sign)
        );
        total_score += last_score;
    }

    // we have to subtract last_score because the last line
    // repeats itself when reading file
    std::cout<<total_score-last_score<<std::endl;

    return 0;
}
