
#include "initialwindow.h"
#include "ui_mainwindow.h"
#include"mainwindow.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include<QtWidgets>

InitialWindow::InitialWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    //initShadowWidget();

            //设置背景颜色
    this->setStyleSheet("QMainWindow {background-color:rgb(255, 150, 30)}");

    QLabel*      inforLabel = new QLabel;
    QLabel*      noteLabel =new QLabel;
    QPushButton* pvpbutton = new QPushButton;
    QPushButton* pvebutton = new QPushButton;

    QFont titleFont("STXingkai",54,75);//设置title的字体为华文行楷
    QFont noteFont("Segoe Script",18,75,true);//设置note的字体为Segoe Script
    QFont buttonFont("STXingkai",20);//设置pushbutton的字体为华文行楷

    //对显示样式（字体及字号）进行设置
    inforLabel->setFont(titleFont);
    inforLabel->setText("五子棋");
    noteLabel->setFont(noteFont);
    noteLabel->setText("                                 SEU\nStuding Everyday University");

    pvpbutton->setFont(buttonFont);
    pvebutton->setFont(buttonFont);
    pvpbutton->setText("双人对战");
    pvebutton->setText("人机对战");

    //同一个SIGNAL可以绑定多个SLOT：
    //实现点击pvpButton的同时跳转页面并初始化本地双人对战游戏
    connect(pvpbutton,SIGNAL(clicked()),SLOT(jumpPage()));
    connect(pvpbutton,SIGNAL(clicked()),SLOT(initPVPGame));

    //实现点击pveButton的同时跳转页面并初始化人机对战游戏；
    connect(pvebutton,SIGNAL(clicked()),SLOT(jumpPage()));
    connect(pvebutton,SIGNAL(clicked()),SLOT(initPVEGame()));

    //设置标题“五子棋”的布局为垂直布局
    QVBoxLayout* cmdLayout = new QVBoxLayout;
    cmdLayout->addWidget(inforLabel,0,Qt::AlignCenter|Qt::AlignHCenter);
    cmdLayout->addWidget(noteLabel,0,Qt::AlignRight|Qt::AlignVCenter);

    //设置模式选择按钮的布局为水平布局
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(pvpbutton,Qt::AlignCenter);
    buttonLayout->addWidget(pvebutton,Qt::AlignCenter);

    //设置总体布局为垂直布局，合并子布局
    QVBoxLayout* mainlayout = new QVBoxLayout;
    mainlayout->addLayout(cmdLayout);
    mainlayout->addLayout(buttonLayout);

    QWidget* m = new QWidget;
    m->setLayout(mainlayout);
    setCentralWidget(m);


            //调用子页面
    //QPushButton *button_tmp = qobject_cast<QPushButton *>(pvebutton);//获取pvp、pve确定模式

   // QStackedWidget stackedWidget;


    //this->close();
    //MainWindow*nextwidget=new MainWindow;//记得改son
    //nextwidget->show();
}

void InitialWindow::jumpPage()
{
    MainWindow* nextPage=new MainWindow;
    this->close();
    nextPage->show();

}

InitialWindow::~InitialWindow() {
    delete ui;
}
