
#include <QtWidgets>
#include "initialform.h"
#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    InitialForm d;
    MainWindow m;
    QObject::connect(&d, &InitialForm::showMainWindow, &m, [&](GameType type){
        m.initGame(type);
        m.show();
        d.hide();
      });
    QObject::connect(&m, &MainWindow::showInitialForm, &d, [&](){
        d.show();
        m.hide();
      });
    d.show();

    return app.exec();
}
