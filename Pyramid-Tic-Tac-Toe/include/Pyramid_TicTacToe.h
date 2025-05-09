#ifndef PYRAMID_TIC_TAC_TOE_H
#define PYRAMID_TIC_TAC_TOE_H

#include "BoardGame_Classes.h"
#include <iomanip>

class MainWindow;


/*--------------------------------Pyramid_TicTacToe_Board Class--------------------------------*/
template<typename T>
class Pyramid_TicTacToe_Board : public Board<T>{
private:
    void initBoard();
public:
    static constexpr T EMPTY = T();
    Pyramid_TicTacToe_Board();
    ~Pyramid_TicTacToe_Board();
    bool update_board(const int& x, const int& y, const T& symbol) override;
    void display_board() const override;
    bool is_win() const override;
    bool is_draw() const override;
    bool game_is_over() const override;
    void cleanUp();
    void resetBoard();

    friend class MainWindow;
};

// Pyramid TTT Constructor
template<typename T>
Pyramid_TicTacToe_Board<T>::Pyramid_TicTacToe_Board() {
    this->rows = 3;
    this->columns = 5;
    this->n_moves = 0;

    initBoard();
}

// Pyramid TTT Destructor
template<typename T>
Pyramid_TicTacToe_Board<T>::~Pyramid_TicTacToe_Board() {
    this->cleanUp();
}

// update_board Function
template<typename T>
bool Pyramid_TicTacToe_Board<T>::update_board(const int& x, const int& y, const T& symbol) {
    if(!(x < 0 || x >= this->rows || y < 0 || y >= this->columns) && (this->board[x][y] == Pyramid_TicTacToe_Board<T>::EMPTY || symbol == Pyramid_TicTacToe_Board<T>::EMPTY)){
        if (symbol == Pyramid_TicTacToe_Board<T>::EMPTY){
            this->n_moves--;
            this->board[x][y] = Pyramid_TicTacToe_Board<T>::EMPTY;
        }
        else {
            this->n_moves++;
            this->board[x][y] = toupper(symbol); // loop better
        }
        return true;
    }
    return false;
}

// display_board Function
template<typename T>
void Pyramid_TicTacToe_Board<T>::display_board() const {
    for (int i = 0; i < this->rows; i++) {
        cout << "\n| ";
        for (int j = 0; j < this->columns; j++) {
            cout << "(" << i << "," << j << ")";
            cout << setw(2) << this->board[i][j] << " |";
        }
        cout << "\n--------------No. of Moves = " << this->n_moves << " -----------------";
    }
    cout << endl;
}

// is_win Function
template<typename T>
bool Pyramid_TicTacToe_Board<T>::is_win() const {
    if(this->board[0][2] == this->board[1][2] && this->board[1][2] == this->board[2][2] && this->board[0][2] != Pyramid_TicTacToe_Board<T>::EMPTY)
        return true;

    if (this->board[1][1] == this->board[1][2] && this->board[1][2] == this->board[1][3] && this->board[1][1] != Pyramid_TicTacToe_Board<T>::EMPTY)
        return true;

    if (this->board[0][2] == this->board[1][3] && this->board[1][3] == this->board[2][4] && this->board[0][2] != Pyramid_TicTacToe_Board<T>::EMPTY)
        return true;

    if (this->board[0][2] == this->board[1][1] && this->board[1][1] == this->board[2][0] && this->board[0][2] != Pyramid_TicTacToe_Board<T>::EMPTY)
        return true;

    for (int i = 0; i < this->columns - 2; ++i) {
        if (this->board[2][i] == this->board[2][i + 1] && this->board[2][i + 1] == this->board[2][i + 2]
            && this->board[2][i] != Pyramid_TicTacToe_Board<T>::EMPTY) {
            return true;
        }
    }

    return false;
}

// is_draw Function
template<typename T>
bool Pyramid_TicTacToe_Board<T>::is_draw() const {
    return (this->n_moves == 9 && !this->is_win());
}


// game_is_over Function
template<typename T>
bool Pyramid_TicTacToe_Board<T>::game_is_over() const {
    return (this->is_win() || this->is_draw());
}


// cleanUp Function
template<typename T>
void Pyramid_TicTacToe_Board<T>::cleanUp() {
    if (this->board) {
        for (int i = 0; i < this->rows; ++i) {
            delete[] this->board[i];
        }
        delete[] this->board;
        this->board = nullptr;
    }

    this->n_moves = 0;
}


// resetBoard Function
template<typename T>
void Pyramid_TicTacToe_Board<T>::resetBoard(){
    this->cleanUp();
    this->initBoard();
}

// initBoard Function
template<typename T>
void Pyramid_TicTacToe_Board<T>::initBoard(){
    this->board = new T*[this->rows];

    for (int i = 0; i < this->rows; ++i) {

        this->board[i] = new T[this->columns];

        for (int j = 0; j < this->columns; ++j) {
            if((i == 0 && (j == 0 || j == 1 || j == 3 || j == 4)) || (i == 1 && (j == 0 || j == 4))){
                this->board[i][j] = '+';
            }
            else this->board[i][j] = Pyramid_TicTacToe_Board<T>::EMPTY;
        }
    }
}

/*--------------------------------Random Player Class--------------------------------*/

/// Random Player Class

template<typename T>
class P_TTT_Random_Player : public RandomPlayer<T>{
public:
    P_TTT_Random_Player(const T& symbol);
    void getmove(int& x, int& y) override;
};

// Random Player Constructor
template<typename T>
P_TTT_Random_Player<T>::P_TTT_Random_Player(const T &symbol) : RandomPlayer<T>(symbol) {
    this->dimension = 3;
    this->name = "Random Computer Player";
    srand(static_cast<unsigned int>(time(0)));
}


// getmove Function [Random Player]
template<typename T>
void P_TTT_Random_Player<T>::getmove(int &x, int &y) {
    x = rand() % 3;
    y = rand() % 5;
}

/*--------------------------------Random Player Class--------------------------------*/
/// Player class
template<typename T>
class P_TTT_Player : public Player<T>{
public:
    P_TTT_Player(string n, T symbol);
    void getmove(int& x, int& y) override;

    friend class MainWindow;
};


// Normal Player Constructor
template<typename T>
P_TTT_Player<T>::P_TTT_Player(string n, T symbol) : Player<T>(n, symbol) {}

// getmove Function [Normal Player]
template<typename T>
void P_TTT_Player<T>::getmove(int &x, int &y) {
    cout << "\nPlease enter your move x and y (0 to 2) separated by spaces: ";
    cin >> x >> y;
}

#endif //PYRAMID_TIC_TAC_TOE_H
