#include "W_TTT_UI.h"

#include <QApplication>
#include <QtGui/QGuiApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Word_Tic_Tac_Toe w;
    w.show();
    return a.exec();
}
