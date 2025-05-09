#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QPushButton>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Pyramid_Tic_Tac_Toe)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/Pyramid Tic Tac Toe.ico"));
    initGrid();

    this->setStyleSheet("background-color: #263238; color: #ECEFF1");


    ui->P_TTT_Grid->setStyleSheet("background-color: #37474F; "
                                  "gridline-color: #546E7A; "
                                  "border: 1px solid #546E7A;");



    player1 = true;
    player2 = false;

    nonHumanPlayerMode = false;
    gameOver = false;

    Board = new Pyramid_TicTacToe_Board<char>();
    players[0] = players[1] = nullptr;

    getPlayerInfo();

    updateNoOfMovesLabel();

    updateState();

}

MainWindow::~MainWindow()
{
    delete ui;
    if (players[0]) delete players[0];
    if (players[1]) delete players[1];
    if (Board) delete Board;
}

void MainWindow::initGrid(){
    ui->P_TTT_Grid->clearContents();
    ui->P_TTT_Grid->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->P_TTT_Grid->setSelectionMode(QAbstractItemView::NoSelection);


    for (int row = 0; row < ui->P_TTT_Grid->rowCount(); ++row) {
        for (int col = 0; col < ui->P_TTT_Grid->columnCount(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem();

            if (isPreDisabled(row, col)) {
                item->setFlags(Qt::NoItemFlags);
                item->setBackground(Qt::transparent);
            } else {
                item->setFlags(Qt::ItemIsEnabled);
                item->setBackground(QColorConstants::Svg::aliceblue);
                item->setTextAlignment(Qt::AlignCenter);
            }
            item->setForeground(Qt::white);
            ui->P_TTT_Grid->setItem(row, col, item);
        }
    }
    ui->P_TTT_Grid->setEnabled(true);
}


bool MainWindow::isPreDisabled(const int& row, const int& col) const{
    return ((row == 0 && (col == 0 || col == 1 || col == 3 || col == 4)) ||
            (row == 1 && (col == 0 || col == 4)));
}

void MainWindow::on_P_TTT_Grid_cellDoubleClicked(int row, int column)
{
    QTableWidgetItem *item = ui->P_TTT_Grid->item(row, column);

    if (!item || item->flags() & Qt::ItemIsEnabled == 0 || !item->text().isEmpty()) {
        return;
    }

    if (player1) {
        Board->update_board(row, column, players[0]->getsymbol());
        updateCell(item, 0, row, column);

    } else if (player2) {
        Board->update_board(row, column, players[1]->getsymbol());
        updateCell(item, 1, row, column);
    }

    isGameIsOver();

    if(gameOver) return void (ui->P_TTT_Grid->setEnabled(false));

    player1 ^= 1;

    updateState();

    if(!nonHumanPlayerMode) player2 ^= 1;

    if(nonHumanPlayerMode)
        nonHumanPlayerTurn(1000);

    updateNoOfMovesLabel();
}

void MainWindow::updateNoOfMovesLabel() const{
    ui->noOfMovesLabel->setText("NUMBER OF MOVES = " + QString::fromStdString(std::to_string(Board->n_moves)));

}


void MainWindow::getPlayerInfo(){

    if (players[0]) delete players[0];
    if (players[1]) delete players[1];


    QString player1Name = QInputDialog::getText(this, "Player 1 Name", "Enter Player 1 name:", QLineEdit::Normal, "Player 1");
    if(player1Name.isEmpty()) player1Name = "Player1";

    QChar player1Symbol = getSymbol("X");

    players[0] = new P_TTT_Player<char>(player1Name.toStdString(), player1Symbol.toLatin1());

    QMessageBox msgBox(this);

    msgBox.setText("Choose your opponent type:");
    QPushButton* aiButton = msgBox.addButton("AI Player", QMessageBox::ActionRole);
    QPushButton* randomButton = msgBox.addButton("Random Computer Player", QMessageBox::ActionRole);
    QPushButton* realButton = msgBox.addButton("Real Player", QMessageBox::ActionRole);

    msgBox.exec();

    QChar player2Symbol;

    if (msgBox.clickedButton() == aiButton) {
        player2Symbol = getSymbol("O");
        players[1] = new P_TTT_AI_Player<char>(player2Symbol.toLatin1());
        players[1]->setBoard(Board);
        nonHumanPlayerMode = true;
    } else if (msgBox.clickedButton() == randomButton) {
        player2Symbol = getSymbol("O");
        players[1] = new P_TTT_Random_Player<char>(player2Symbol.toLatin1());
        nonHumanPlayerMode = true;
    } else if (msgBox.clickedButton() == realButton) {
        QString player2Name = QInputDialog::getText(this, "Player 2 Name", "Enter Player 2 name:", QLineEdit::Normal, "Player 2");
        if (player2Name.isEmpty()) player2Name = "Player 2";

        player2Symbol = getSymbol("O");

        players[1] = new P_TTT_Player<char>(player2Name.toStdString(), player2Symbol.toLatin1());
    }

    ui->name1Label->setText("Name: " + QString::fromStdString(players[0]->getname()));
    ui->mark1Label->setText("Mark: " + QString::fromStdString(string(1, players[0]->getsymbol())));

    ui->name2label->setText("Name: " + QString::fromStdString(players[1]->getname()));
    ui->mark2Label->setText("Mark: " + QString::fromStdString(string(1, players[1]->getsymbol())));
}


void MainWindow::executeNonHumanPlayerTurn(){
    player1 = false;

    int x, y;
    players[1]->getmove(x, y);

    while(!Board->update_board(x, y, players[1]->getsymbol())){
        players[1]->getmove(x, y);
    }

    QTableWidgetItem *item = ui->P_TTT_Grid->item(x, y);

    updateCell(item, 1, x, y);

    isGameIsOver();

    player1 = true;

    updateState();

    ui->P_TTT_Grid->setEnabled(!gameOver);

}

void MainWindow::nonHumanPlayerTurn(const int& delay = 2000) {
    ui->P_TTT_Grid->setEnabled(false);
    QTimer::singleShot(delay, this, &MainWindow::executeNonHumanPlayerTurn);
}


void MainWindow::updateCell(QTableWidgetItem *item, const int& playerIndex, const int& row, const int& column){
    item->setText(QString::fromStdString(std::string(1, players[playerIndex]->getsymbol())));
    item->setTextAlignment(Qt::AlignCenter);
    item->setFont(QFont("Outrun future", 45, QFont::Bold));
    if(!playerIndex) item->setBackground(Qt::blue);
    else item->setBackground(Qt::red);
    item->setForeground(Qt::white);

    item->setFlags(Qt::NoItemFlags);

}


void MainWindow::updateState(){
    if(player1){
        ui->state2Label->setText("State: Waiting...");
        ui->state1label->setText("State: YOUR TURN!");
    }
    else{
        ui->state1label->setText("State: Waiting...");
        ui->state2Label->setText("State: YOUR TURN!");
    }
}


void MainWindow::playAgain(){
    Board->resetBoard();

    player1 = true, player2 = false, gameOver = false;
    nonHumanPlayerMode = false;

    getPlayerInfo();

    initGrid();    

    updateNoOfMovesLabel();

    updateState();
}

void MainWindow::isGameIsOver(){
    if (Board->game_is_over()) {
        QString msg;
        if (Board->is_win()) {
            if (player1)
                msg = QString::fromStdString(players[0]->getname() + " has won.");
            else
                msg = QString::fromStdString(players[1]->getname() + " has won.");
        } else if (Board->is_draw()) {
            msg = "The match ended with a draw.";
        }

        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Game Over!");
        msgBox.setText(msg);
        msgBox.setIcon(QMessageBox::Information);

        QPushButton* playAgainButton = msgBox.addButton("Play Again", QMessageBox::AcceptRole);
        QPushButton* quitButton = msgBox.addButton("Quit", QMessageBox::RejectRole);


        msgBox.exec();

        if(msgBox.clickedButton() == playAgainButton){
            playAgain();
        } else if (msgBox.clickedButton() == quitButton) {
            QApplication::quit();
        } else if (msgBox.clickedButton() == nullptr){
             gameOver = true;
        }
    }
}

QChar MainWindow::getSymbol(const QString& defaultSymbol){
    QChar playerSymbol;
    while (true) {
        QString symbolInput = QInputDialog::getText(this, "Player 1 Symbol", "Enter Player 1 symbol (one character):", QLineEdit::Normal, defaultSymbol);
        if (symbolInput.size() == 1 && !symbolInput.at(0).isSpace()) {
            playerSymbol = symbolInput.at(0);
            break;
        } else {
            QMessageBox::warning(this, "Invalid Input", "Player 1 symbol must be a single non-whitespace character.");
        }
    }
    return playerSymbol;
}
