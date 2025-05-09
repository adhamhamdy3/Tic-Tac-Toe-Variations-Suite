#ifndef TOUCHKEYBOARDDIALOG_H
#define TOUCHKEYBOARDDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>

class TouchKeyboardDialog : public QDialog {
    Q_OBJECT

public:
    explicit TouchKeyboardDialog(QWidget *parent = nullptr);

signals:
    void characterSelected(QChar character);

private slots:
    void onButtonClicked();

private:
    void createKeyboard();
};

#endif // TOUCHKEYBOARDDIALOG_H
