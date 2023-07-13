#include "initialform.h"
#include "mainwindow.h"
#include <QtWidgets>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    InitialForm w;
    w.show();

    return app.exec();
}
