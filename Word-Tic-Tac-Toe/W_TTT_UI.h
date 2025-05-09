#ifndef W_TTT_UI_H
#define W_TTT_UI_H

#include <QMainWindow>
#include "BoardGame_Classes.h"
#include "Word_TicTacToe.h"
#include "touchkeyboarddialog.h"
#include <QTableWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class Word_Tic_Tac_Toe;
}
QT_END_NAMESPACE

class Word_Tic_Tac_Toe : public QMainWindow
{
    Q_OBJECT

public:
    Word_Tic_Tac_Toe(QWidget *parent = nullptr);
    ~Word_Tic_Tac_Toe();

private slots:
    void on_W_TTT_Grid_cellDoubleClicked(int row, int column);

private:
    Ui::Word_Tic_Tac_Toe *ui;
    Player<char>* players[2];
    Word_TicTacToe_Board<char>* Board;
    GameManager<char>* W_TTT_GAME;

private:
    bool player1, player2;

    bool nonHumanPlayerMode;
    bool gameOver;

    void getPlayerInfo();
    QChar getSymbol(const QString&);

    void updateCell(QTableWidgetItem *item, const int& playerIndex, const int& row, const int& column);

    void isGameIsOver();

    void updateState();
    void updateNoOfMovesLabel() const;

    void executeNonHumanPlayerTurn();

    void nonHumanPlayerTurn(const int&);

    void initGrid();

    void playAgain();
};
#endif // W_TTT_UI_H
