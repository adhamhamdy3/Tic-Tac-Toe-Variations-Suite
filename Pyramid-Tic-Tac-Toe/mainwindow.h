#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "BoardGame_Classes.h"
#include "Pyramid_TicTacToe.h"
#include "P_TTT_AI_Player.h"
#include <QTableWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class Pyramid_Tic_Tac_Toe;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_P_TTT_Grid_cellDoubleClicked(int row, int column);

private:
    Ui::Pyramid_Tic_Tac_Toe *ui;
    Player<char>* players[2];
    Pyramid_TicTacToe_Board<char>* Board;
private:
    void initGrid();
    bool isPreDisabled(const int&, const int&) const;
    void updateNoOfMovesLabel() const;

    void getPlayerInfo();

    bool player1, player2;

    bool nonHumanPlayerMode;
    bool gameOver;

    void nonHumanPlayerTurn(const int&);
    void executeNonHumanPlayerTurn();

    void AI_PlayerTurn(const int&);
    void execute_AI_PlayerTurn();

    void updateState();

    void isGameIsOver();

    void updateCell(QTableWidgetItem*, const int&, const int&, const int&);

    void playAgain();

    QChar getSymbol(const QString&);

};
#endif // MAINWINDOW_H
