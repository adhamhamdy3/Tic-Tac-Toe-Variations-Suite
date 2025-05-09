#ifndef Word_TIC_TAC_TOE_H
#define Word_TIC_TAC_TOE_H

#include "BoardGame_Classes.h"
#include <iomanip>
#include <map>
#include <fstream>

class Word_Tic_Tac_Toe;


/*--------------------------------Word_TicTacToe_Board Class--------------------------------*/
template<typename T>
class Word_TicTacToe_Board : public Board<T>{
private:
    void initBoard();
    map<string, bool> validWords;
public:
    static constexpr T EMPTY = T();
    Word_TicTacToe_Board();
    ~Word_TicTacToe_Board();
    bool update_board(const int& x, const int& y, const T& letter) override;
    void display_board() override;
    bool is_win() override;
    bool is_draw() override;
    bool game_is_over() override;
    void cleanUp();
    void resetBoard();
};

// Word TTT Constructor
template<typename T>
Word_TicTacToe_Board<T>::Word_TicTacToe_Board() {
    this->rows = 3;
    this->columns = 3;
    this->n_moves = 0;

    initBoard();
}

// Word TTT Destructor
template<typename T>
Word_TicTacToe_Board<T>::~Word_TicTacToe_Board() {
    this->cleanUp();
}

// update_board Function
template<typename T>
bool Word_TicTacToe_Board<T>::update_board(const int& x, const int& y, const T& letter) {
    if(!(x < 0 || x >= this->rows || y < 0 || y >= this->columns) && (this->board[x][y] == Word_TicTacToe_Board<T>::EMPTY || letter == Word_TicTacToe_Board<T>::EMPTY)){
        if (letter == Word_TicTacToe_Board<T>::EMPTY){
            this->n_moves--;
            this->board[x][y] = Word_TicTacToe_Board<T>::EMPTY;
        }
        else {
            this->n_moves++;
            this->board[x][y] = toupper(letter); // loop better
        }
        return true;
    }
    return false;
}

// display_board Function
template<typename T>
void Word_TicTacToe_Board<T>::display_board() {
    for (int i = 0; i < this->rows; i++) {
        cout << "\n| ";
        for (int j = 0; j < this->columns; j++) {
            cout << "(" << i << "," << j << ")";
            cout << setw(2) << this->board[i][j] << " |";
        }
        cout << "\n-----------------------------";
    }
    cout << endl;
}

// is_win Function
template<typename T>
bool Word_TicTacToe_Board<T>::is_win() {
    // Check rows
    for (int i = 0; i < this->rows; i++) {
        string row = string(1, this->board[i][0]) +
                     string(1, this->board[i][1]) +
                     string(1, this->board[i][2]);
        if (validWords[row])
            return true;
    }
    for (int i = 0; i < this->columns; ++i) {
        string col = string(1, this->board[0][i]) +
                     string(1, this->board[1][i]) +
                     string(1, this->board[2][i]);
        if (validWords[col])
            return true;
    }

    string diagonal1 = string(1, this->board[0][0]) +
                       string(1, this->board[1][1]) +
                       string(1, this->board[2][2]);

    string diagonal2 = string(1, this->board[0][2]) +
                       string(1, this->board[1][1]) +
                       string(1, this->board[2][0]);

    if (validWords[diagonal1] || validWords[diagonal2])
        return true;

    return false;
}

// is_draw Function
template<typename T>
bool Word_TicTacToe_Board<T>::is_draw() {
    return (this->n_moves == 9 && !this->is_win());
}


// game_is_over Function
template<typename T>
bool Word_TicTacToe_Board<T>::game_is_over()  {
    return (this->is_win() || this->is_draw());
}


// cleanUp Function
template<typename T>
void Word_TicTacToe_Board<T>::cleanUp() {
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
void Word_TicTacToe_Board<T>::resetBoard(){
    this->cleanUp();
    this->initBoard();
}

// initBoard Function
template<typename T>
void Word_TicTacToe_Board<T>::initBoard(){
    this->board = new T*[this->rows];

    for (int i = 0; i < this->rows; ++i) {

        this->board[i] = new T[this->columns];

        for (int j = 0; j < this->columns; ++j) {
            this->board[i][j] = Word_TicTacToe_Board<T>::EMPTY;
        }
    }

    fstream loadValidWords("dic.txt", ios::in);
    if (!loadValidWords){
        cerr << "File could not open!" << endl;
        exit(EXIT_FAILURE);
    }

    string carry;
    while (!loadValidWords.eof()){
        loadValidWords >> carry;
        validWords[carry] = true;
    }
}

/*--------------------------------Random Player Class--------------------------------*/

/// Random Player Class

template<typename T>
class W_TTT_Random_Player : public RandomPlayer<T>{
public:
    T newRandomLetter;
    W_TTT_Random_Player(const T& letter);
    void getmove(int& x, int& y) override;

    friend class Word_Tic_Tac_Toe;
};

// Random Player Constructor
template<typename T>
W_TTT_Random_Player<T>::W_TTT_Random_Player(const T &letter) : RandomPlayer<T>(letter) {
    this->dimension = 3;
    this->name = "Random Computer Player";
    srand(static_cast<unsigned int>(time(0)));
}


// getmove Function [Random Player]
template<typename T>
void W_TTT_Random_Player<T>::getmove(int &x, int &y) {
    x = rand() % 3;
    y = rand() % 3;
    this->symbol = 'A' + (rand() % 26);
}

/*--------------------------------Player Class--------------------------------*/
/// Player class
template<typename T>
class W_TTT_Player : public Player<T>{
public:
    T newLetter;
    W_TTT_Player(string n, T letter);
    void getmove(int& x, int& y) override;

    friend class Word_Tic_Tac_Toe;
};


// Normal Player Constructor
template<typename T>
W_TTT_Player<T>::W_TTT_Player(string n, T letter) : Player<T>(n, letter) {}

// getmove Function [Normal Player]
template<typename T>
void W_TTT_Player<T>::getmove(int &x, int &y) {
    cout << "\nEnter your letter:";
    cin >> this->symbol;
    cout << "\nPlease enter your X Y coordenates (0 to 2) separated by spaces: ";
    cin >> x >> y;
}

#endif //Word_TIC_TAC_TOE_H
