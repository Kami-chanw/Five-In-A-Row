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
const static size_t defaultSingleTime = 12000;

MainWindow::MainWindow(QWidget* parent) : ShadowWidget<QMainWindow>(parent) {
    // init window
    setFixedSize(QSize(840, 700));
    QGridLayout* mainLayout = new QGridLayout;

    static auto gameOverHandler = [=](const QString& str) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("");
        msgBox.setText(QString("%1 想要再来一局吗？").arg(str));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);

        int ret = msgBox.exec();
        if (ret == QMessageBox::Yes) {
            initGame(chessBoard->gameType());
        }
        else if (ret == QMessageBox::No) {
            emit showInitialForm();
        }
    };

    // init main layout
    playerA = new Avatar(this, Avatar::LabelPosition::Left, QImage(":/imgs/R-C.jpg"), "Player A",
                         defaultTotalTime, defaultSingleTime);
    playerB = new Avatar(this, Avatar::LabelPosition::Right, QImage(":/imgs/R-C.jpg"), "Player B",
                         defaultTotalTime, defaultSingleTime);
    connect(playerA, &Avatar::timeout, this, [=] {
        playerB->stopTimer();
        gameOverHandler(QString("%1超时，恭喜%2获胜!").arg(playerA->name(), playerB->name()));
    });
    connect(playerB, &Avatar::timeout, this, [=] {
        playerA->stopTimer();
        gameOverHandler(QString("%1超时，恭喜%2获胜!").arg(playerA->name(), playerB->name()));
    });
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
        if (playerB->isActive())
            playerB->stopTimer(), playerA->startTimer();
        else
            playerA->stopTimer(), playerB->startTimer();
    });
    connect(chessBoard, &ChessBoard::gameOver, this, [=](bool isWin) {
        playerA->stopTimer();
        playerB->stopTimer();
        if (isWin)
            gameOverHandler(
                QString("恭喜%1获胜！").arg((playerA->isActive() ? playerA : playerB)->name()));
        else {
            gameOverHandler("游戏已死局");
        }
    });
    initGame(GameType::PVE);
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

void MainWindow::initGame(GameType type) {
    playerA->resetLeftTime();
    playerB->resetLeftTime();
    chessBoard->initGame(type);
    if (type == GameType::PVE)
        playerB->setName("Robot");
    else
        playerB->setName("Player B");
}
