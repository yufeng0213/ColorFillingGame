#include "ColorFillingGame.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ColorFillingGame w;
    w.show();
    return a.exec();
}
