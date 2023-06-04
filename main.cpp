#include"initialwindow.h"

#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    //MainWindow   w;
    InitialWindow i;
    i.show();
    return a.exec();
}
