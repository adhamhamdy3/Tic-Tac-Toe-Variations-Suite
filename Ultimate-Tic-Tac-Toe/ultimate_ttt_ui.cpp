#include "ultimate_ttt_ui.h"
#include "./ui_ultimate_ttt_ui.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QTimer>

Ultimate_TTT_UI::Ultimate_TTT_UI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Ultimate_TTT_UI)
{
    ui->setupUi(this);

    this->setStyleSheet("background-color: #263238; color: #ECEFF1;");

    player1 = true;
    player2 = false;

    gameOver = false;
    borderOn = true;

    nonHumanPlayerMode = false;

    ultimateBoard = new Ultimate_Board<char>();
    players[0] = players[1] = nullptr;

    getPlayersInfo();

    UI_labels[0][0] = ui->_0_0_Label;
    UI_labels[0][1] = ui->_0_1_Label;
    UI_labels[0][2] = ui->_0_2_Label;
    UI_labels[1][0] = ui->_1_0_Label;
    UI_labels[1][1] = ui->_1_1_Label;
    UI_labels[1][2] = ui->_1_2_Label;
    UI_labels[2][0] = ui->_2_0_Label;
    UI_labels[2][1] = ui->_2_1_Label;
    UI_labels[2][2] = ui->_2_2_Label;

    UI_grids[0][0] = ui->_0_0_Grid;
    UI_grids[0][1] = ui->_0_1_Grid;
    UI_grids[0][2] = ui->_0_2_Grid;
    UI_grids[1][0] = ui->_1_0_Grid;
    UI_grids[1][1] = ui->_1_1_Grid;
    UI_grids[1][2] = ui->_1_2_Grid;
    UI_grids[2][0] = ui->_2_0_Grid;
    UI_grids[2][1] = ui->_2_1_Grid;
    UI_grids[2][2] = ui->_2_2_Grid;

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            // UI_grids[row][col]->setStyleSheet("QTableWidget::item {"
            //                                   "border: 1px solid #546E7A;"
            //                                   "}"
            //                                   "border: 1px solid #546E7A;"
            //                                   "background-color: #37474F;"
            //                                   "gridline-color: #546E7A;");
            UI_grids[row][col]->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            UI_grids[row][col]->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            UI_labels[row][col]->setAttribute(Qt::WA_TransparentForMouseEvents);
            UI_labels[row][col]->lower();
        }

    }

    blinkTimer = new QTimer(this);
    connect(blinkTimer, &QTimer::timeout, this, &Ultimate_TTT_UI::toggleBlinkingBorder);

    originalStyle = UI_grids[0][0]->styleSheet();

    turnON_ALL();
    updateNoOfMovesLabel();
}

Ultimate_TTT_UI::~Ultimate_TTT_UI()
{
    delete ui;

    if (players[0]) delete players[0];
    if (players[1]) delete players[1];
    if (ultimateBoard) delete ultimateBoard;
}

void Ultimate_TTT_UI::getPlayersInfo(){
    if (players[0]) delete players[0];
    if (players[1]) delete players[1];

    QString player1Name = QInputDialog::getText(this, "Player 1 Name", "Enter Player 1 name:", QLineEdit::Normal, "Player 1");
    if(player1Name.isEmpty()) player1Name = "Player1";

    QChar player1Symbol = getSymbol("X");

    players[0] = new Ultimate_TTT_Player<char>(player1Name.toStdString(), player1Symbol.toLatin1(), ultimateBoard);

    QMessageBox msgBox(this);

    msgBox.setText("Choose your opponent type:");
    QPushButton* randomButton = msgBox.addButton("Random Computer Player", QMessageBox::ActionRole);
    QPushButton* realButton = msgBox.addButton("Real Player", QMessageBox::ActionRole);

    msgBox.exec();

    QChar player2Symbol;

   if (msgBox.clickedButton() == randomButton) {
        player2Symbol = getSymbol("O");
        players[1] = new Ultimate_TTT_Random_Player<char>(player2Symbol.toLatin1(), ultimateBoard);
        nonHumanPlayerMode = true;
    } else if (msgBox.clickedButton() == realButton) {
        QString player2Name = QInputDialog::getText(this, "Player 2 Name", "Enter Player 2 name:", QLineEdit::Normal, "Player 2");
        if (player2Name.isEmpty()) player2Name = "Player 2";

        player2Symbol = getSymbol("O");

        players[1] = new Ultimate_TTT_Player<char>(player2Name.toStdString(), player2Symbol.toLatin1(), ultimateBoard);
    }

    ui->name1_Label->setText("Name: " + QString::fromStdString(players[0]->getname()));
    ui->mark1_Label->setText("Mark: " + QString::fromStdString(string(1, players[0]->getsymbol())));

    ui->name2_Label->setText("Name: " + QString::fromStdString(players[1]->getname()));
    ui->mark2_Label->setText("Mark: " + QString::fromStdString(string(1, players[1]->getsymbol())));
}

QChar Ultimate_TTT_UI::getSymbol(const QString& defaultSymbol){
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

void Ultimate_TTT_UI::turnON_OFF(const int& i, const int& j, bool state) {
    QString overlayBorderStyle = state ? "border: 2px solid red;" : "";

    QString newStyle = originalStyle + overlayBorderStyle;
    UI_grids[i][j]->setStyleSheet(newStyle);

    UI_grids[i][j]->setEnabled(state);
}


void Ultimate_TTT_UI::turnON_ALL(){
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            turnON_OFF(i, j, true);
        }
    }
}

void Ultimate_TTT_UI::turnOFF_ALL(){
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            turnON_OFF(i, j, false);
        }
    }
}

void Ultimate_TTT_UI::keepCurrentBoard(){
    if (ultimateBoard->currentBoard_X == -1 || ultimateBoard->currentBoard_Y == -1) return;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (i == ultimateBoard->currentBoard_X && j == ultimateBoard->currentBoard_Y) {
                turnON_OFF(i, j, true);
            } else {
                turnON_OFF(i, j, false);
            }
        }
    }

    // borderOn = true;
    // toggleBlinkingBorder();
    // blinkTimer->start(1100);
}

void Ultimate_TTT_UI::toggleBlinkingBorder() {
    QString borderStyle = borderOn ? "2px solid red" : "none";
    UI_grids[ultimateBoard->currentBoard_X][ultimateBoard->currentBoard_Y]->setStyleSheet(originalStyle + QString("border: %1;").arg(borderStyle));
    borderOn = !borderOn;
}

void Ultimate_TTT_UI::updateCell(QTableWidgetItem* item, const int& playerIndex, const int& board_X, const int& board_Y, const int& row, const int& column){
    item->setText(QString::fromStdString(std::string(1, players[playerIndex]->getsymbol())));
    item->setTextAlignment(Qt::AlignCenter);
    item->setFont(QFont("Outrun future", 30, QFont::Bold));

    if(!playerIndex) item->setBackground(Qt::blue);
    else item->setBackground(Qt::red);

    item->setForeground(Qt::white);
    item->setFlags(Qt::NoItemFlags);

    // QWidget* cellWidget = UI_grids[board_X][board_Y]->cellWidget(row, column);

    // if (!cellWidget) {
    //     cellWidget = new QWidget();
    //     UI_grids[board_X][board_Y]->setCellWidget(row, column, cellWidget);
    // }

    // cellWidget->setStyleSheet("border: 3px solid yellow;");

    // QTimer::singleShot(500, [cellWidget]() {
    //     cellWidget->setStyleSheet("");
    // });
}

void Ultimate_TTT_UI::initGrids(){
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            UI_grids[i][j]->clearContents();
            UI_grids[i][j]->setEditTriggers(QAbstractItemView::NoEditTriggers);
            UI_grids[i][j]->setSelectionMode(QAbstractItemView::NoSelection);

            for (int row = 0; row < 3; ++row) {
                for (int column = 0; column < 3; ++column) {
                    QTableWidgetItem* cell = UI_grids[i][j]->item(row, column);

                    if (!cell) {
                        cell = new QTableWidgetItem();
                        UI_grids[i][j]->setItem(row, column, cell);
                    }

                    cell->setText("");
                    cell->setTextAlignment(Qt::AlignCenter);
                    cell->setFont(QFont("Outrun future", 30, QFont::Bold));
                    cell->setFlags(Qt::ItemIsEnabled);
                }
            }

        }
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            UI_labels[i][j]->clear();
            UI_labels[i][j]->setStyleSheet("");
            UI_labels[i][j]->setAttribute(Qt::WA_TransparentForMouseEvents, false);
            UI_labels[i][j]->lower();
        }

    }
    turnON_ALL();
}


void Ultimate_TTT_UI::playAgain(){
    ultimateBoard->resetUltimateBoard();

    player1 = true, player2 = false, gameOver = false;
    nonHumanPlayerMode = false;

    getPlayersInfo();

    initGrids();

    updateNoOfMovesLabel();

    updateState();
}

void Ultimate_TTT_UI::isGameIsOver(){
    if (ultimateBoard->game_is_over()) {
        QString msg;
        if (ultimateBoard->is_win()) {
            if (player1)
                msg = QString::fromStdString(players[0]->getname() + " has won.");
            else
                msg = QString::fromStdString(players[1]->getname() + " has won.");
        } else if (ultimateBoard->is_draw()) {
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


void Ultimate_TTT_UI::updateState(){
    if(player1){
        ui->state2_Label->setText("State: Waiting...");
        ui->state1_Label->setText("State: YOUR TURN!");
    }
    else{
        ui->state1_Label->setText("State: Waiting...");
        ui->state2_Label->setText("State: YOUR TURN!");
    }

    if(ultimateBoard->canPickBoard){
        ui->msgLabel->setText("You can choose any Tic Tac Toe grid you want to pick your cell.");
        ui->msgLabel->setFont(QFont("Outrun future", 8, QFont::Bold));
    }

    else{
        QString X = QString::fromStdString(std::to_string(ultimateBoard->currentBoard_X));
        QString Y = QString::fromStdString(std::to_string(ultimateBoard->currentBoard_Y));
        ui->msgLabel->setText("You can only pick from (" + X + ", " + Y + ") Tic Tac Toe grid.");
        ui->msgLabel->setFont(QFont("Outrun future", 8, QFont::Bold));

    }
}


void Ultimate_TTT_UI::switchBoards(){
    if (ultimateBoard->canPickBoard) {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                turnON_OFF(i, j, (ultimateBoard->localWinners[i][j] == Local_Board<char>::EMPTY));
            }
        }
    } else {
        keepCurrentBoard();
    }
}

void Ultimate_TTT_UI::operate(QTableWidgetItem* item, const int& row, const int& column, const int& board_X, const int& board_Y){

    if(ultimateBoard->canPickBoard){
        ultimateBoard->currentBoard_X = board_X;
        ultimateBoard->currentBoard_Y = board_Y;
        switchBoards();
        ultimateBoard->canPickBoard = false;
    }

    int recentBoard_X = ultimateBoard->currentBoard_X;
    int recentBoard_Y = ultimateBoard->currentBoard_Y;
    int pIndx;

    if (player1) {
        ultimateBoard->update_board(row, column, players[0]->getsymbol());
        updateCell(item, 0, board_X, board_Y, row, column);
        pIndx = 0;
    } else if (player2) {
        ultimateBoard->update_board(row, column, players[1]->getsymbol());
        updateCell(item, 1, board_X, board_Y, row, column);
        pIndx = 1;
    }

    if(ultimateBoard->boards[recentBoard_X][recentBoard_Y]->winner != Local_Board<char>::EMPTY){
        updateGridWinner(board_X, board_Y, pIndx);
    }

    isGameIsOver();

    if(gameOver) return void (turnOFF_ALL());

    switchBoards();

    player1 ^= 1;

    updateState();

    if(!nonHumanPlayerMode) player2 ^= 1;

    if(nonHumanPlayerMode)
        nonHumanPlayerTurn(1000);

    updateNoOfMovesLabel();
}

void Ultimate_TTT_UI::on__0_0_Grid_cellDoubleClicked(int row, int column)
{
    if(!ui->_0_0_Grid->isEnabled()) return;

    QTableWidgetItem *item = ui->_0_0_Grid->item(row, column);

    operate(item, row, column, 0, 0);
}


void Ultimate_TTT_UI::on__0_1_Grid_cellDoubleClicked(int row, int column)
{
    if(!ui->_0_1_Grid->isEnabled()) return;

    QTableWidgetItem *item = ui->_0_1_Grid->item(row, column);

    operate(item, row, column, 0, 1);
}


void Ultimate_TTT_UI::on__0_2_Grid_cellDoubleClicked(int row, int column)
{
    if(!ui->_0_2_Grid->isEnabled()) return;

    QTableWidgetItem *item = ui->_0_2_Grid->item(row, column);

    operate(item, row, column, 0, 2);
}


void Ultimate_TTT_UI::on__1_0_Grid_cellDoubleClicked(int row, int column)
{
    if(!ui->_1_0_Grid->isEnabled()) return;

    QTableWidgetItem *item = ui->_1_0_Grid->item(row, column);

    operate(item, row, column, 1, 0);
}


void Ultimate_TTT_UI::on__1_1_Grid_cellDoubleClicked(int row, int column)
{
    if(!ui->_1_1_Grid->isEnabled()) return;

    QTableWidgetItem *item = ui->_1_1_Grid->item(row, column);

    operate(item, row, column, 1, 1);
}


void Ultimate_TTT_UI::on__1_2_Grid_cellDoubleClicked(int row, int column)
{
    if(!ui->_1_2_Grid->isEnabled()) return;

    QTableWidgetItem *item = ui->_1_2_Grid->item(row, column);

    operate(item, row, column, 1, 2);
}


void Ultimate_TTT_UI::on__2_0_Grid_cellDoubleClicked(int row, int column)
{
    if(!ui->_2_0_Grid->isEnabled()) return;

    QTableWidgetItem *item = ui->_2_0_Grid->item(row, column);

    operate(item, row, column, 2, 0);
}


void Ultimate_TTT_UI::on__2_1_Grid_cellDoubleClicked(int row, int column)
{
    if(!ui->_2_1_Grid->isEnabled()) return;

    QTableWidgetItem *item = ui->_2_1_Grid->item(row, column);

    operate(item, row, column, 2, 1);
}


void Ultimate_TTT_UI::on__2_2_Grid_cellDoubleClicked(int row, int column)
{
    if(!ui->_2_2_Grid->isEnabled()) return;

    QTableWidgetItem *item = ui->_2_2_Grid->item(row, column);

    operate(item, row, column, 2, 2);
}

void Ultimate_TTT_UI::updateNoOfMovesLabel() const{
    ui->noOfMoves_Label->setText("NUMBER OF MOVES = " +
                                 QString::fromStdString(std::to_string(ultimateBoard->n_moves)));

}

void Ultimate_TTT_UI::updateGridWinner(int gridX, int gridY, int playerIndex) {
    QLabel* label = UI_labels[gridX][gridY];

    if (!label) return;

    label->setText(QString(players[playerIndex]->getsymbol()));
    label->setFont(QFont("Outrun future", 100, QFont::Bold));
    label->setAlignment(Qt::AlignCenter);

    QString baseStyle = QString(
                            "QLabel { "
                            "    background-color: %1; "
                            "    color: white; "
                            "} "
                            "QLabel:hover { "
                            "    background-color: rgba(0, 0, 0, 0); "
                            "    color: rgba(255, 255, 255, 0.8); "
                            "    transform: scale(1.1); "
                            "}")
                            .arg(playerIndex == 0 ? "blue" : "red");

    label->setStyleSheet(baseStyle);

    label->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    label->raise();
    label->show();
}



void Ultimate_TTT_UI::executeNonHumanPlayerTurn(){

    player1 = false;

    int x, y, board_X, board_Y;
    players[1]->getmove(x, y);
    board_X = ultimateBoard->currentBoard_X; board_Y = ultimateBoard->currentBoard_Y;


    while(!ultimateBoard->update_board(x, y, players[1]->getsymbol())){
        players[1]->getmove(x, y);
        board_X = ultimateBoard->currentBoard_X; board_Y = ultimateBoard->currentBoard_Y;
    }

    QTableWidgetItem *item = UI_grids[board_X][board_Y]->item(x, y);

    updateCell(item, 1, board_X, board_Y, x, y);

    if(ultimateBoard->boards[board_X][board_Y]->winner != Local_Board<char>::EMPTY){
        updateGridWinner(board_X, board_Y, 1);
    }

    isGameIsOver();

    player1 = true;

    updateState();

    if(gameOver){
        turnOFF_ALL();
        return;
    }

    switchBoards();
    updateNoOfMovesLabel();
}


void Ultimate_TTT_UI::nonHumanPlayerTurn(const int &delay){
    turnOFF_ALL();
    QTimer::singleShot(delay, this, &Ultimate_TTT_UI::executeNonHumanPlayerTurn);
}

