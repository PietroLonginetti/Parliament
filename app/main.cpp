#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Parliament");
    w.resize(1000,700);
    w.show();
    return a.exec();
}
