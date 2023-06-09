#include "mainwindow.h"
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget* parent) : ShadowWidget<QMainWindow>(parent) {
    setBackgroundColor(Qt::white);
    QWidget* centralWidget = new QWidget();
    QHBoxLayout* mainLayout = new QHBoxLayout();
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
    initShadowWidget(mainLayout);
    title()->setButtons(TitleBar::Close | TitleBar::Minimize);
    setBackgroundColor(Qt::blue);
}

MainWindow::~MainWindow() {}
