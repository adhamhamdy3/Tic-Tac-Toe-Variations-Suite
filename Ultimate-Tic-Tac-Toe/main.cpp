#include "ultimate_ttt_ui.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Ultimate_TTT_UI w;
    w.show();
    return a.exec();
}
