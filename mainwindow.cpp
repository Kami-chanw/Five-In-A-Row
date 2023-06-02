
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include"son"
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    //设置背景颜色
    this->setStyleSheet("QMainWindow {background-color:rgb(255, 150, 30)}");

    QLabel*      inforLabel = new QLabel;
    QPushButton* pvpbutton = new QPushButton;
    QPushButton* pvebutton = new QPushButton;

    inforLabel->setText("五子棋");
    pvpbutton->setText("PVP");
    pvebutton->setText("PVE");

    QHBoxLayout* cmdLayout = new QHBoxLayout;
    cmdLayout->addWidget(inforLabel);

    QHBoxLayout* buttonlayout = new QHBoxLayout;
    cmdLayout->addWidget(pvpbutton);
    cmdLayout->addWidget(pvebutton);

    QVBoxLayout* mainlayout = new QVBoxLayout;
    mainlayout->addLayout(cmdLayout);
    mainlayout->addLayout(buttonlayout);

    QWidget* m = new QWidget;
    m->setLayout(mainlayout);
    setCentralWidget(m);


    //调用子页面
    QPushButton *button_tmp = qobject_cast<QPushButton *>(obj);//获取pvp、pve确定模式
    this->close();
    son*nextwidget=new son;//记得改son
    nextwidget->show();
}

MainWindow::~MainWindow() {
    delete ui;
}
