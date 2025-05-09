#ifndef _BOARDGAME_CLASSES_H
#define _BOARDGAME_CLASSES_H

#include <string>
using namespace std;

class Ultimate_TTT_UI;

template <typename T>
class Board {
protected:
    int rows, columns;
    T** board;
    int n_moves = 0;

public:
    /// Return true if move is valid and put it on the localBoards
    /// within localBoards boundaries in an empty cell
    /// Return false otherwise
    virtual bool update_board(const int& x, const int& y, const T& symbol) = 0;

    /// Display the localBoards and the pieces on it
    virtual void display_board() = 0;

    /// Returns true if there is any ultimateWinner
    virtual bool is_win() = 0;

    /// Return true if all moves are done and no ultimateWinner
    virtual bool is_draw() = 0;

    /// Return true if the game is over
    virtual bool game_is_over() = 0;

    friend class Ultimate_TTT_UI;
};

template <typename T>
class Player {
protected:
    string name;
    T symbol;
    Board<T>* boardPtr;  // Pointer to the localBoards
public:
    /// Two constructors to initiate players
    /// Give the player a symbol to use in playing
    /// It can be X, O, or others
    /// Optionally, you can give them a name
    Player(const string& n, const T& symbol);
    Player(const T& symbol); // For computer players

    virtual void getmove(int& x, int& y) = 0;
    T getsymbol() const;
    string getname() const;
    void setBoard(Board<T>* b);

    virtual ~Player() = default;
};

/// This class represents a random computer player
/// that generates random positions x, y
/// If invalid, the game manager asks to regenerate
template <typename T>
class RandomPlayer : public Player<T> {
protected:
    int dimension;
public:
    // Take a symbol and pass it to the parent
    RandomPlayer(const T& symbol);
    // Generate a random move
    virtual void getmove(int& x, int& y) = 0;
};

class Ultimate_TTT_UI;


template <typename T>
class GameManager {
private:
    Board<T>* boardPtr;
    Player<T>* players[2];
public:
    GameManager(Board<T>*, Player<T>* playerPtr[2]);
    ~GameManager();
    void run();

    friend class Ultimate_TTT_UI;

};


//--------------------------------------- IMPLEMENTATION

#include <iostream>
using namespace std;

template <typename T>
GameManager<T>::GameManager(Board<T>* bPtr, Player<T>* playerPtr[2]) {
    boardPtr = bPtr;
    players[0] = playerPtr[0];
    players[1] = playerPtr[1];
}

template <typename T>
GameManager<T>::~GameManager() {
    players[0] = nullptr;
    players[1] = nullptr;
}

template <typename T>
void GameManager<T>::run() {
    int x, y;

    if (!boardPtr)
        exit(EXIT_FAILURE);

    boardPtr->display_board();

    while (!boardPtr->game_is_over()) {
        for (int i : {0, 1}) {
            players[i]->getmove(x, y);
            while (!boardPtr->update_board(x, y, players[i]->getsymbol())) {
                players[i]->getmove(x, y);
            }
            boardPtr->display_board();
            if (boardPtr->is_win()) {
                cout << players[i]->getname() << " wins\n";
                return;
            }
            if (boardPtr->is_draw()) {
                cout << "Draw!\n";
                return;
            }
        }
    }
}


using namespace std;
// Constructor for Player with a name and symbol
template <typename T>
Player<T>::Player(const std::string& n, const T& symbol) {
    this->name = n;
    this->symbol = symbol;
}

// Constructor for Player with only a symbol (e.g., for computer players)
template <typename T>
Player<T>::Player(const T& symbol) {
    this->name = "Computer";
    this->symbol = symbol;
}

// Constructor for RandomPlayer, passing the symbol to the parent Player class
template <typename T>
RandomPlayer<T>::RandomPlayer(const T& symbol) : Player<T>(symbol) {}

// Getter for player's name
template <typename T>
std::string Player<T>::getname() const {
    return this->name;
}

// Getter for player's symbol
template <typename T>
T Player<T>::getsymbol() const {
    return this->symbol;
}

// Method to set the localBoards pointer in the Player class
template <typename T>
void Player<T>::setBoard(Board<T>* b) {
    this->boardPtr = b;
}

#endif //_BOARDGAME_CLASSES_H

