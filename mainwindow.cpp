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

#include "Game.h"
#include "avatarwidget.h"
#include "chessboard.h"
#include <QSpacerItem>
#include <QTime>

const static QTime  defaultTotalTime(0, 5, 0);
const static size_t defaultSingleTime = 12000;

MainWindow::MainWindow(GameType type, QWidget* parent) : ShadowWidget<QMainWindow>(parent) {
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
            initGame();
        }
        else if (ret == QMessageBox::No) {
            emit showInitialForm();
        }
    };

    // init main layout
    playerA = new AvatarWidget(this, AvatarWidget::Left, QImage(":/imgs/R-C.jpg"), "Player A", defaultTotalTime,
                         defaultSingleTime);
    playerB = new AvatarWidget(this, AvatarWidget::Right, QImage(":/imgs/R-C.jpg"), "Player B",
                         defaultTotalTime, defaultSingleTime);

    if (type == GameType::PVE)
        playerB->setName("Robot");
    else
        playerB->setName("Player B");
    connect(playerA, &AvatarWidget::timeout, this, [=] {
        playerA->stopTimer();
        playerB->stopTimer();
        playerA->setAvatar(AvatarWidget::Dead);
        gameOverHandler(QString("%1超时，恭喜%2获胜!").arg(playerA->name(), playerB->name()));
    });
    connect(playerB, &AvatarWidget::timeout, this, [=] {
        playerA->stopTimer();
        playerB->stopTimer();
        playerB->setAvatar(AvatarWidget::Dead);
        gameOverHandler(QString("%1超时，恭喜%2获胜!").arg(playerB->name(), playerA->name()));
    });
    playerA->setFixedHeight(80);
    playerB->setFixedHeight(80);
    mainLayout->addItem(new QSpacerItem(30, 20, QSizePolicy::Expanding), 0, 0);
    mainLayout->addWidget(playerA, 0, 1, Qt::AlignRight);
    mainLayout->addWidget(new QLabel("VS"), 0, 2);
    mainLayout->addWidget(playerB, 0, 3, Qt::AlignLeft);
    mainLayout->addItem(new QSpacerItem(30, 20, QSizePolicy::Expanding), 0, 4);
    // init chess board
    chessBoard = new ChessBoard(type);
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
    chessBoard->initGame();
    mainLayout->addWidget(chessBoard, 1, 0, 1, 5, Qt::AlignCenter);
    initShadowWidget(mainLayout);
    setResizable(false);
    title()->setButtons(TitleBar::Close | TitleBar::Minimize);
    title()->setIconColor(QColor(0x212121));
    setBackgroundColor(QColor(0xF8F9FA));
}

MainWindow::~MainWindow() {
    if (chessBoard) {
        delete chessBoard;
    }
}

void MainWindow::closeEvent(QCloseEvent* event) {
    Q_UNUSED(event);
    playerA->stopTimer();
    playerB->stopTimer();
    emit showInitialForm();
}

void MainWindow::initGame() {
    playerA->init();
    playerB->init();
    chessBoard->initGame();
}
