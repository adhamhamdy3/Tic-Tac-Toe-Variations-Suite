#include "touchkeyboarddialog.h"

TouchKeyboardDialog::TouchKeyboardDialog(QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Touch Keyboard");
    setFixedSize(300, 300);

    createKeyboard();
}

void TouchKeyboardDialog::createKeyboard() {
    QGridLayout *gridLayout = new QGridLayout(this);

    QString keys = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    int row = 0, col = 0;

    for (int i = 0; i < keys.size(); ++i) {
        QPushButton *button = new QPushButton(QString(keys[i]), this);
        button->setFixedSize(40, 40);
        gridLayout->addWidget(button, row, col);

        connect(button, &QPushButton::clicked, this, &TouchKeyboardDialog::onButtonClicked);

        col++;
        if (col == 6) {
            col = 0;
            row++;
        }
    }

    setLayout(gridLayout);
}

void TouchKeyboardDialog::onButtonClicked() {
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        emit characterSelected(button->text().at(0));
        close();
    }
}
