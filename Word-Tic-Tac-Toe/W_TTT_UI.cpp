#include "W_TTT_UI.h"
#include "./ui_W_TTT_UI.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QTimer>

Word_Tic_Tac_Toe::Word_Tic_Tac_Toe(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Word_Tic_Tac_Toe)
{
    ui->setupUi(this);

    this->setStyleSheet("background-color: #263238; color: #ECEFF1");


    ui->W_TTT_Grid->setStyleSheet("background-color: #37474F; "
                                  "gridline-color: #546E7A; "
                                  "border: 1px solid #546E7A;");

    player1 = true;
    player2 = false;

    nonHumanPlayerMode = false;
    gameOver = false;

    Board = new Word_TicTacToe_Board<char>();

    getPlayerInfo();

    initGrid();

    updateNoOfMovesLabel();

    updateState();
}

Word_Tic_Tac_Toe::~Word_Tic_Tac_Toe()
{
    delete ui;

    if (players[0]) delete players[0];
    if (players[1]) delete players[1];
    if (Board) delete Board;
}

void Word_Tic_Tac_Toe::initGrid(){
    ui->W_TTT_Grid->clearContents();
    ui->W_TTT_Grid->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->W_TTT_Grid->setSelectionMode(QAbstractItemView::NoSelection);


    for (int row = 0; row < ui->W_TTT_Grid->rowCount(); ++row) {
        for (int col = 0; col < ui->W_TTT_Grid->columnCount(); ++col) {

            QTableWidgetItem *item = new QTableWidgetItem();
            item->setFlags(Qt::ItemIsEnabled);
            item->setBackground(QColorConstants::Svg::aliceblue);
            item->setTextAlignment(Qt::AlignCenter);
            item->setForeground(Qt::white);
            ui->W_TTT_Grid->setItem(row, col, item);
        }
    }
    ui->W_TTT_Grid->setEnabled(true);
}

void Word_Tic_Tac_Toe::getPlayerInfo(){
    QString player1Name = QInputDialog::getText(this, "Player 1 Name", "Enter Player 1 name:", QLineEdit::Normal, "Player 1");
    if(player1Name.isEmpty()) player1Name = "Player1";

    players[0] = new W_TTT_Player<char>(player1Name.toStdString(), 'X');

    QMessageBox msgBox(this);

    msgBox.setText("Choose your opponent type:");
    // QPushButton* aiButton = msgBox.addButton("AI Player", QMessageBox::ActionRole);
    QPushButton* randomButton = msgBox.addButton("Random Computer Player", QMessageBox::ActionRole);
    QPushButton* realButton = msgBox.addButton("Real Player", QMessageBox::ActionRole);

    msgBox.exec();

    /*if (msgBox.clickedButton() == aiButton) {
        players[1] = new P_TTT_AI_Player<char>(player2Symbol.toLatin1());
        players[1]->setBoard(Board);
        nonHumanPlayerMode = true;
    } else */if (msgBox.clickedButton() == randomButton) {
        players[1] = new W_TTT_Random_Player<char>('O');
        nonHumanPlayerMode = true;
    } else if (msgBox.clickedButton() == realButton) {
        QString player2Name = QInputDialog::getText(this, "Player 2 Name", "Enter Player 2 name:", QLineEdit::Normal, "Player 2");
        if (player2Name.isEmpty()) player2Name = "Player 2";

        players[1] = new W_TTT_Player<char>(player2Name.toStdString(), 'O');
    }

    ui->name1Label->setText("Name: " + QString::fromStdString(players[0]->getname()));
    // ui->mark1Label->setText("Mark: " + QString::fromStdString(string(1, players[0]->getsymbol())));

    ui->name2Label->setText("Name: " + QString::fromStdString(players[1]->getname()));
    // ui->mark2Label->setText("Mark: " + QString::fromStdString(string(1, players[1]->getsymbol())));
}

void Word_Tic_Tac_Toe::on_W_TTT_Grid_cellDoubleClicked(int row, int column){

    QTableWidgetItem *item = ui->W_TTT_Grid->item(row, column);

    if (!item || !(item->flags() & Qt::ItemIsEnabled) || !item->text().isEmpty()){
        return;
    }

    QChar selectedChar;

    TouchKeyboardDialog *keyboardDialog = new TouchKeyboardDialog(this);
    connect(keyboardDialog, &TouchKeyboardDialog::characterSelected, this, [&](QChar character) {
        // updateCell(row, column, character);
        selectedChar = character;
    });

    keyboardDialog->exec();

    if(selectedChar.isNull()) return;

    if (player1) {
        players[0]->symbol = selectedChar.toLatin1();
        Board->update_board(row, column, players[0]->getsymbol());
        updateCell(item, 0, row, column);

    } else if (player2) {
        players[1]->symbol = selectedChar.toLatin1();
        Board->update_board(row, column, players[1]->getsymbol());
        updateCell(item, 1, row, column);
    }

    updateNoOfMovesLabel();

    isGameIsOver();

    if(gameOver) return void (ui->W_TTT_Grid->setEnabled(false));

    player1 ^= 1;

    updateState();

    if(!nonHumanPlayerMode) player2 ^= 1;

    if(nonHumanPlayerMode)
        nonHumanPlayerTurn(750);

}

void Word_Tic_Tac_Toe::updateCell(QTableWidgetItem *item, const int& playerIndex, const int& row, const int& column){
    item->setText(QString::fromStdString(std::string(1, players[playerIndex]->getsymbol())));
    item->setTextAlignment(Qt::AlignCenter);
    item->setFont(QFont("Outrun future", 45, QFont::Bold));
    if(!playerIndex) item->setBackground(Qt::blue);
    else item->setBackground(Qt::red);
    item->setForeground(Qt::white);

    item->setFlags(Qt::NoItemFlags);

}

void Word_Tic_Tac_Toe::playAgain(){
    Board->resetBoard();

    player1 = true, player2 = false, gameOver = false;
    nonHumanPlayerMode = false;

    getPlayerInfo();

    initGrid();

    updateNoOfMovesLabel();

    updateState();
}

void Word_Tic_Tac_Toe::isGameIsOver(){
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

void Word_Tic_Tac_Toe::updateState(){
    if(player1){
        ui->state2Label->setText("State: Waiting...");
        ui->state1Label->setText("State: YOUR TURN!");
    }
    else{
        ui->state1Label->setText("State: Waiting...");
        ui->state2Label->setText("State: YOUR TURN!");
    }
}

void Word_Tic_Tac_Toe::updateNoOfMovesLabel() const{
    ui->noOfMovesLabel->setText("NUMBER OF MOVES = " + QString::fromStdString(std::to_string(Board->n_moves)));

}

void Word_Tic_Tac_Toe::executeNonHumanPlayerTurn(){
    player1 = false;

    int x, y;
    players[1]->getmove(x, y);

    while(!Board->update_board(x, y, players[1]->getsymbol())){
        players[1]->getmove(x, y);
    }

    QTableWidgetItem *item = ui->W_TTT_Grid->item(x, y);

    updateCell(item, 1, x, y);

    isGameIsOver();

    player1 = true;

    updateState();

    ui->W_TTT_Grid->setEnabled(!gameOver);
}


void Word_Tic_Tac_Toe::nonHumanPlayerTurn(const int & delay){
    ui->W_TTT_Grid->setEnabled(false);
    QTimer::singleShot(delay, this, &Word_Tic_Tac_Toe::executeNonHumanPlayerTurn);
    updateNoOfMovesLabel();
}
