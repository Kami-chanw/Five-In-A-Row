#include "mainwindow.h"
#include <QAction>
#include <QDebug>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include <QVBoxLayout>
#include <math.h>

//#include"Portrait.h"
#include "Game.h"
#include "avatar.h"
#include "chessboard.h"
#include <QSpacerItem>
#include <QTime>
#include <QTimer>
// #include "ui_mainwindow.h"

const static QTime  defaultTotalTime(0, 5, 0);
const static size_t defaultSingleTime = 5000;

MainWindow::MainWindow(QWidget* parent) : ShadowWidget<QMainWindow>(parent) {
    // init window
    setFixedSize(QSize(900, 680));
    QGridLayout* mainLayout = new QGridLayout;

    // init main layout
    Avatar *playerA = new Avatar(this, Avatar::LabelPosition::Left, QImage(":/imgs/R-C.jpg"),
                                 "Player A", defaultTotalTime, defaultSingleTime),
           *playerB = new Avatar(this, Avatar::LabelPosition::Right, QImage(":/imgs/R-C.jpg"),
                                 "Player B", defaultTotalTime, defaultSingleTime);
    playerA->setFixedHeight(80);
    playerB->setFixedHeight(80);
    mainLayout->addItem(new QSpacerItem(30, 20, QSizePolicy::Expanding), 0, 0);
    mainLayout->addWidget(playerA, 0, 1, Qt::AlignRight);
    mainLayout->addWidget(new QLabel("VS"), 0, 2);
    mainLayout->addWidget(playerB, 0, 3, Qt::AlignLeft);
    mainLayout->addItem(new QSpacerItem(30, 20, QSizePolicy::Expanding), 0, 4);
    // init chess board
    chessBoard = new ChessBoard;
    connect(chessBoard, &ChessBoard::makeMove, this, [=] {
        if (playerA->isActive())
            playerA->stopTimer(), playerB->startTimer();
        else
            playerB->stopTimer(), playerA->startTimer();
    });
    connect(chessBoard, &ChessBoard::gameOver, this, [=]{

      });
    mainLayout->addWidget(chessBoard, 1, 0, 1, 5, Qt::AlignCenter);
    initShadowWidget(mainLayout);
    setResizable(false);
    title()->setButtons(TitleBar::Close);
    title()->setIconColor(QColor(0x21, 0x21, 0x21));
    setBackgroundColor(QColor(0xF8, 0xF9, 0xFA));
}

MainWindow::~MainWindow() {
    if (chessBoard) {
        delete chessBoard;
    }
}

void MainWindow::initGame(GameType type) { chessBoard->initGame(type); }
