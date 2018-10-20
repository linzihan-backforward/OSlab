#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.move(200,200);
    //w.resize(650,478);
    w.setFixedSize(650,478);
    w.show();

    return a.exec();
}
