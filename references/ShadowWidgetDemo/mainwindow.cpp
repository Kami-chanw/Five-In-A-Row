#include "mainwindow.h"
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget* parent) : ShadowWidget<QMainWindow>(parent) {
    setBackgroundColor(Qt::white);
    QHBoxLayout* mainLayout = new QHBoxLayout();
    setLayout(mainLayout);
    initShadowWidget(mainLayout);
    title()->setButtons(TitleBar::Close | TitleBar::Minimize);
    setBackgroundColor(Qt::blue);
}

MainWindow::~MainWindow() {}
