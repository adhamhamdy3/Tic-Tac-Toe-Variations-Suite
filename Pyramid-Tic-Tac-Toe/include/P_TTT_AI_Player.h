#ifndef P_TTT_AI_PLAYER_H
#define P_TTT_AI_PLAYER_H

#include "BoardGame_Classes.h"
#include "Pyramid_TicTacToe.h"

template <typename T>
class P_TTT_AI_Player : public Player<T>{
private:
    pair<int, int> bestMove();
    int miniMax(const T& symbol, bool isMaximizing);
public:
    P_TTT_AI_Player(const T&);
    void getmove(int& x, int& y) override;
};

template<typename T>
int P_TTT_AI_Player<T>::miniMax(const T& symbol, bool isMaximizing) {
    if (this->boardPtr->is_win()) return (isMaximizing ? 1 : -1);
    if (this->boardPtr->is_draw()) return 0;

    int bestScore = isMaximizing ? INT_MIN : INT_MAX;
    T opponentSymbol = (this->symbol == 'X') ? 'O' : 'X';

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (this->boardPtr->update_board(i, j, symbol)){
                int score = miniMax(opponentSymbol, !isMaximizing);
                this->boardPtr->update_board(i, j, Pyramid_TicTacToe_Board<T>::EMPTY);

                if (isMaximizing) bestScore = max(bestScore, score);
                else bestScore = min(bestScore, score);
            }
        }
    }
    return bestScore;
}

template<typename T>
void P_TTT_AI_Player<T>::getmove(int &x, int &y) {
    pair<int, int> move = bestMove();
    x = move.first;
    y = move.second;
}

template<typename T>
pair<int, int> P_TTT_AI_Player<T>::bestMove() {
    int bestScore = INT_MIN;
    pair<int, int> Move = {-1, -1};

    T opponentSymbol = (this->symbol == 'X') ? 'O' : 'X';

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (this->boardPtr->update_board(i, j, this->symbol)) {
                if (this->boardPtr->is_win()) {
                    this->boardPtr->update_board(i, j, Pyramid_TicTacToe_Board<T>::EMPTY);
                    return {i, j};
                }
                this->boardPtr->update_board(i, j, Pyramid_TicTacToe_Board<T>::EMPTY);
            }
        }
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (this->boardPtr->update_board(i, j, opponentSymbol)) {
                if (this->boardPtr->is_win()) {
                    this->boardPtr->update_board(i, j, Pyramid_TicTacToe_Board<T>::EMPTY);
                    return {i, j};
                }
                this->boardPtr->update_board(i, j, Pyramid_TicTacToe_Board<T>::EMPTY);
            }
        }
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (this->boardPtr->update_board(i, j, this->symbol)){ // check if the cell is empty
                int score = miniMax(this->symbol, true);
                this->boardPtr->update_board(i, j, Pyramid_TicTacToe_Board<T>::EMPTY); // undo Move that minimax caused

                if (score > bestScore){
                    bestScore = score;
                    Move = {i, j};
                }
            }

        }
    }
    return Move;
}

template<typename T>
P_TTT_AI_Player<T>::P_TTT_AI_Player(const T &symbol) : Player<T>(symbol) {
    this->name = "AI Player";
}

#endif //P_TTT_AI_PLAYER_H
