#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    w.move((QApplication::desktop()->width() - 864)/2,(QApplication::desktop()->height() - 720)/2);

    return a.exec();
}
