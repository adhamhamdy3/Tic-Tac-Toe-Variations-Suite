#ifndef ULTIMATE_TTT_UI_H
#define ULTIMATE_TTT_UI_H

#include <QMainWindow>
#include "BoardGame_Classes.h"
#include "Ultimate_Tic_Tac_Toe.h"
#include <QTableWidgetItem>
#include <QLabel>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui {
class Ultimate_TTT_UI;
}
QT_END_NAMESPACE

class Ultimate_TTT_UI : public QMainWindow
{
    Q_OBJECT

public:
    Ultimate_TTT_UI(QWidget *parent = nullptr);
    ~Ultimate_TTT_UI();

private slots:
    void on__0_0_Grid_cellDoubleClicked(int row, int column);

    void on__0_1_Grid_cellDoubleClicked(int row, int column);

    void on__0_2_Grid_cellDoubleClicked(int row, int column);

    void on__1_0_Grid_cellDoubleClicked(int row, int column);

    void on__1_1_Grid_cellDoubleClicked(int row, int column);

    void on__1_2_Grid_cellDoubleClicked(int row, int column);

    void on__2_0_Grid_cellDoubleClicked(int row, int column);

    void on__2_1_Grid_cellDoubleClicked(int row, int column);

    void on__2_2_Grid_cellDoubleClicked(int row, int column);

    void toggleBlinkingBorder();

private:
    Ui::Ultimate_TTT_UI *ui;
    Player<char>* players[2];
    Ultimate_Board<char>* ultimateBoard;
    //GameManager<char>* ULTIMATE_TTT_GAME;

private:
    QLabel* UI_labels[3][3];
    QTableWidget* UI_grids[3][3];
    QMap<QTableWidgetItem*, QString> cellOriginalStyles;

    QTimer *blinkTimer;

    bool player1, player2;

    bool gameOver;

    bool nonHumanPlayerMode;

    bool firstTime;

    bool borderOn;

    void getPlayersInfo();

    QChar getSymbol(const QString& defaultSymbol);

    QString originalStyle;

    void keepCurrentBoard();

    void turnON_OFF(const int&, const int&, bool);
    void turnON_ALL();
    void turnOFF_ALL();

    void updateCell(QTableWidgetItem*, const int&, const int&, const int&, const int&, const int&);

    void isGameIsOver();

    void switchBoards();

    void playAgain();

    void operate(QTableWidgetItem*, const int&, const int&, const int&, const int&);

    void updateNoOfMovesLabel() const;

    void updateState();

    void updateGridWinner(int gridX, int gridY, int playerIndex);

    QTableWidget* getGridPtr(const int& i, const int& j);

    void nonHumanPlayerTurn(const int&);

    void executeNonHumanPlayerTurn();

    void initGrids();
};
#endif // ULTIMATE_TTT_UI_H
