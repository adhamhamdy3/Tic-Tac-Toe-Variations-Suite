#ifndef ULTIMATE_TIC_TAC_TOE_H
#define ULTIMATE_TIC_TAC_TOE_H

#include "BoardGame_Classes.h"
#include <iomanip>

template<typename T>
class Local_Board : public Board<T> {
private:
    void initBoard();
public:
    static constexpr T EMPTY = T();
    T winner;
    Local_Board();
    ~Local_Board();
    bool update_board(const int& x, const int& y, const T& symbol) override;
    void display_board() override;
    bool is_win() override;
    bool is_draw() override;
    bool game_is_over() override;
    void cleanUp();
    void resetBoard();
};

template<typename T>
bool Local_Board<T>::game_is_over() {
    return this->is_win() || this->is_draw();

}

template<typename T>
bool Local_Board<T>::is_draw() {
    return (this->n_moves == 9 && !(this->is_win()));

}

template<typename T>
bool Local_Board<T>::is_win() {
    for (int i = 0; i < this->rows; i++) {
        if (this->board[i][0] != Local_Board<T>::EMPTY &&
            this->board[i][0] == this->board[i][1] &&
            this->board[i][1] == this->board[i][2]) {
            this->winner = this->board[i][0];
            return true;
        }
        if (this->board[0][i] != Local_Board<T>::EMPTY &&
            this->board[0][i] == this->board[1][i] &&
            this->board[1][i] == this->board[2][i]) {
            this->winner = this->board[0][i];
            return true;
        }
    }

    if (this->board[1][1] != Local_Board<T>::EMPTY) {
        if (this->board[0][0] == this->board[1][1] &&
            this->board[1][1] == this->board[2][2]) {
            this->winner = this->board[1][1];
            return true;
        }
        if (this->board[0][2] == this->board[1][1] &&
            this->board[1][1] == this->board[2][0]) {
            this->winner = this->board[1][1];
            return true;
        }
    }

    return false;
}


template<typename T>
void Local_Board<T>::display_board() {
    for (int i = 0; i < this->rows; i++) {
        cout << "\n|| ";
        for (int j = 0; j < this->columns; j++) {
            cout << "(" << i << "," << j << ")";
            cout << setw(2) << this->board[i][j] << " ||";
        }
        cout << "\n-----------------------------";
    }
    //cout << endl;
}

template<typename T>
bool Local_Board<T>::update_board(const int &x, const int &y, const T &symbol) {
    if(!(x < 0 || x >= this->rows || y < 0 || y >= this->columns) && (this->board[x][y] == Local_Board<T>::EMPTY || symbol == Local_Board<T>::EMPTY)){
        if (symbol == Local_Board<T>::EMPTY){
            this->n_moves--;
            this->board[x][y] = Local_Board<T>::EMPTY;
        }
        else {
            this->n_moves++;
            this->board[x][y] = toupper(symbol); // loop better
        }
        return true;
    }
    return false;
}

template<typename T>
void Local_Board<T>::resetBoard(){
    this->cleanUp();
    this->initBoard();
}

template<typename T>
void Local_Board<T>::cleanUp() {
    if (this->board) {
        for (int i = 0; i < this->rows; ++i) {
            if (this->board[i]) delete[] this->board[i];
        }
        delete[] this->board;
        this->board = nullptr;
    }
}


template<typename T>
void Local_Board<T>::initBoard() {
    this->rows = this->columns = 3;
    this->n_moves = 0;
    this->winner = Local_Board<T>::EMPTY;

    this->board = new T*[this->rows];
    for (int i = 0; i < this->rows; ++i) {
        this->board[i] = new T[this->columns];
        for (int j = 0; j < this->columns; ++j) {
            this->board[i][j] = Local_Board<T>::EMPTY;
        }
    }
}


template<typename T>
Local_Board<T>::~Local_Board() {
    this->cleanUp();
}

template<typename T>
Local_Board<T>::Local_Board() {
    initBoard();
}

/*==================================================================================================*/

template<typename T>
class Ultimate_Board : public Board<T> {
public:
    T ultimateWinner;
    Local_Board<T>* boards[3][3];
public:
    int currentBoard_X, currentBoard_Y;
    T localWinners[3][3];
    Ultimate_Board();
    ~Ultimate_Board();
    bool canPickBoard;
    bool update_board(const int& x, const int& y, const T& symbol) override;
    void display_board() override;
    bool is_win() override;
    bool is_draw() override;
    bool game_is_over() override;
    bool pickBoard(int, int);
    bool isEmpty() const;
    void cleanUp();
    void initLocalBoards();
    void resetUltimateBoard();
};

template<typename T>
bool Ultimate_Board<T>::isEmpty() const{
    return this->currentBoard_X == -1 && this->currentBoard_Y == -1;
}

template<typename T>
bool Ultimate_Board<T>::pickBoard(int board_X, int board_Y) {
    if(this->boards[board_X][board_Y]->game_is_over()){
        return false;
    }
    this->currentBoard_X = board_X;
    this->currentBoard_Y = board_Y;
    this->canPickBoard = false;
    return true;
}

template<typename T>
bool Ultimate_Board<T>::is_draw() {
    bool draw = true;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if(this->localWinners[i][j] == Local_Board<T>::EMPTY){
                draw = false;
            }
        }
    }

    return ((this->n_moves == 81) || (!this->is_win() && draw));
}

template<typename T>
bool Ultimate_Board<T>::game_is_over() {
    return this->is_win() || this->is_draw();
}

template<typename T>
bool Ultimate_Board<T>::is_win() {
    for (int i = 0; i < this->rows; i++) {
        if ((this->localWinners[i][0] == this->localWinners[i][1] && this->localWinners[i][1] == this->localWinners[i][2] && this->localWinners[i][0] != Local_Board<T>::EMPTY) ||
            (this->localWinners[0][i] == this->localWinners[1][i] && this->localWinners[1][i] == this->localWinners[2][i] && this->localWinners[0][i] != Local_Board<T>::EMPTY)) {

            this->ultimateWinner = this->localWinners[i][0];
            return true;
        }
    }

    if ((this->localWinners[0][0] == this->localWinners[1][1] && this->localWinners[1][1] == this->localWinners[2][2] && this->localWinners[0][0] != Local_Board<T>::EMPTY) ||
        (this->localWinners[0][2] == this->localWinners[1][1] && this->localWinners[1][1] == this->localWinners[2][0] && this->localWinners[0][2] != Local_Board<T>::EMPTY)) {

        this->ultimateWinner = this->localWinners[1][1];
        return true;
    }

    return false;
}

template<typename T>
void Ultimate_Board<T>::display_board() {
    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < this->columns; ++j) {
            this->boards[i][j]->display_board();
        }
        cout << "==========================================================================================" << endl;
    }
    cout << endl;
}

template<typename T>
bool Ultimate_Board<T>::update_board(const int &x, const int &y, const T &symbol) {
    bool moveMade = this->boards[this->currentBoard_X][this->currentBoard_Y]->update_board(x, y, symbol);

    if(!moveMade) return false;

    if (this->boards[this->currentBoard_X][this->currentBoard_Y]->is_win()){
        this->localWinners[this->currentBoard_X][this->currentBoard_Y] = this->boards[this->currentBoard_X][this->currentBoard_Y]->winner;
    }
    this->currentBoard_X = x;
    this->currentBoard_Y = y;

    if(this->boards[this->currentBoard_X][this->currentBoard_Y]->game_is_over())
        this->canPickBoard = true;

    this->n_moves++;

    return true;
}

template<typename T>
void Ultimate_Board<T>::resetUltimateBoard(){
    this->cleanUp();
    this->initLocalBoards();
}

template<typename T>
void Ultimate_Board<T>::initLocalBoards(){
    this->rows = this->columns = 3;
    this->canPickBoard = true;
    this->currentBoard_X = this->currentBoard_Y = -1;
    this->board = nullptr;

    this->ultimateWinner = Local_Board<T>::EMPTY;

    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < this->columns; ++j) {
            this->boards[i][j] = new Local_Board<T>();
            this->localWinners[i][j] = Local_Board<T>::EMPTY;
        }
    }
    this->n_moves = 0;
}

template<typename T>
void Ultimate_Board<T>::cleanUp() {
    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < this->columns; ++j) {
            if (this->boards[i][j]) {
                this->boards[i][j]->cleanUp();
                delete this->boards[i][j];
                this->boards[i][j] = nullptr;
            }
        }
    }
}


template<typename T>
Ultimate_Board<T>::~Ultimate_Board() {
    this->cleanUp();
}

template<typename T>
Ultimate_Board<T>::Ultimate_Board() {
    this->initLocalBoards();
}

/*=======================================================================================*/

template<typename T>
class Ultimate_TTT_Player : public Player<T>{
public:
    Ultimate_Board<T>* ultimateBoardPtr;
    Ultimate_TTT_Player(const string& name, const T& s, Ultimate_Board<T>*);
    void getmove(int &x, int &y) override;
};

template<typename T>
void Ultimate_TTT_Player<T>::getmove(int &x, int &y) {

    if (this->ultimateBoardPtr->canPickBoard) {
        cout << "You can pick any local board you want to pick a cell from." << endl;
        int x, y;
        cin >> x >> y;
        while (!this->ultimateBoardPtr->pickBoard(x, y)){
            cout << "You can pick any local board you want to pick a cell from." << endl;
            cin >> x >> y;
        }
    } else {
        cout << "You are allowed to pick from local board at coordinates ("
             << this->ultimateBoardPtr->currentBoard_X << ", " << this->ultimateBoardPtr->currentBoard_Y << ") only.";
        cout << "\nPlease enter your move x and y (0 to 2) separated by spaces: ";
        cin >> x >> y;
    }

}


template<typename T>
Ultimate_TTT_Player<T>::Ultimate_TTT_Player(const string &name, const T &s, Ultimate_Board<T>* ultimateBoardPtr) : Player<T>(name, s) {
    this->boardPtr = nullptr;
    this->ultimateBoardPtr = ultimateBoardPtr;
}
/*===================================================================================================================*/

template<typename T>
class Ultimate_TTT_Random_Player : public RandomPlayer<T>{
private:
    Ultimate_Board<T>* ultimateBoardPtr;
public:
    Ultimate_TTT_Random_Player(const T& symbol, Ultimate_Board<T>* ultimateBoardPtr);
    void getmove(int& x, int& y) override;
};

template<typename T>
void Ultimate_TTT_Random_Player<T>::getmove(int &x, int &y) {
    x = rand() % this->dimension;
    y = rand() % this->dimension;

    int boardX = -1, boardY = -1;

    if (this->ultimateBoardPtr->canPickBoard) {
        do {
            boardX = rand() % this->dimension;
            boardY = rand() % this->dimension;
        } while (!this->ultimateBoardPtr->pickBoard(boardX, boardY));
    }
}


template<typename T>
Ultimate_TTT_Random_Player<T>::Ultimate_TTT_Random_Player(const T &symbol, Ultimate_Board<T> *ultimateBoardPtr)
    : RandomPlayer<T>(symbol) {
    this->dimension = 3;
    this->name = "Random Computer Player";
    srand(static_cast<unsigned int>(time(0)));

    this->boardPtr = nullptr;

    this->ultimateBoardPtr = ultimateBoardPtr;
}


#endif //ULTIMATE_TIC_TAC_TOE_H
