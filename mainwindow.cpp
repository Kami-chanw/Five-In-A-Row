#include "mainwindow.h"
#include <QAction>
#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include <math.h>
// #include "ui_mainwindow.h"

const int chessRadius = 16;       // 棋子半径
const int gridSize = 48;          // 棋盘网格边长
const int chessBoardMargin = 48;  // 页面边界距棋盘边缘空隙
const int posRange = 24;          // 鼠标点击的最大模糊边界距离
const int markSizeBefore = 8;     // 预期落子位置标记的边长
const int markSizeAfter = 8;      // 当前落子标记的边长
const int aiChessDelay = 600;     // AI下棋前的思考时间

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
//, ui(new Ui::MainWindow)
{
    // ui->setupUi(this);

    // 设置棋盘界面大小
    setFixedSize(chessBoardMargin * 2 + gridSize * chessBoardSize, chessBoardMargin * 2 + gridSize * chessBoardSize);

    // 设置鼠标追踪功能
    setMouseTracking(true);

    // 添加选项菜单
    QMenu*   gameMenu = menuBar()->addMenu(tr("Game"));  // menuBar默认是存在的，直接加菜单就可以了
    QAction* actionPVP = new QAction("Person VS Person", this);
    connect(actionPVP, SIGNAL(triggered()), this, SLOT(initPVPGame()));
    gameMenu->addAction(actionPVP);

    QAction* actionPVE = new QAction("Person VS Computer", this);
    connect(actionPVE, SIGNAL(triggered()), this, SLOT(initPVEGame()));
    gameMenu->addAction(actionPVE);

    // 初始化游戏
    initGame();
}

MainWindow::~MainWindow() {
    if (game) {
        delete game;
        game = nullptr;  // 对game(ptr)置空
    }
    // delete ui;
}

void MainWindow::initGame() {
    // 初始化游戏模型
    game = new Game;
    initPVPGame();
}

void MainWindow::initPVPGame() {
    gameType_ = PVP;
    game->gameStatus = ONGOING;
    game->startGame(gameType_);
    update();
}

void MainWindow::initPVEGame() {
    gameType_ = PVE;
    game->gameStatus = ONGOING;
    game->startGame(gameType_);
    update();
}

void MainWindow::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    // 绘制棋盘
    painter.setRenderHint(QPainter::Antialiasing, true);  // 设置窗口抗锯齿
    // 绘制棋盘上的线条
    QPen pen;
    pen.setWidth(2);  // 设置绘制线条宽度
    painter.setPen(pen);

    for (int num = 0; num < chessBoardSize + 1; num++) {
        // 绘制棋盘网格竖线
        painter.drawLine(chessBoardMargin + gridSize * num, chessBoardMargin,                     // 画线起点坐标
                         chessBoardMargin + gridSize * num, size().height() - chessBoardMargin);  // 画线终点坐标
        // 绘制棋盘网格横线
        painter.drawLine(chessBoardMargin, chessBoardMargin + gridSize * num, size().width() - chessBoardMargin,
                         chessBoardMargin + gridSize * num);
    }

    // 填充棋盘以及棋子的颜色
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);  // 设置颜色填充的模式

    // 绘制预期棋子落子位置标记
    if (clickRowPos > 0 && clickRowPos < chessBoardSize
        && clickColPos > 0 && clickColPos < chessBoardSize
        && game->gameMap[clickRowPos][clickColPos]==0) {
        if (game->player)
            brush.setColor((Qt::black));
        else
            brush.setColor(Qt::white);
        painter.setBrush(brush);

        painter.drawRect(chessBoardMargin + gridSize * clickColPos - markSizeBefore / 2,
                         chessBoardMargin + gridSize * clickRowPos - gridSize / 2, markSizeBefore, markSizeBefore);
    }

    // 绘制棋子
    for (int i = 0; i < chessBoardSize; i++) {
        for (int j = 0; j < chessBoardSize; j++) {
            if (1 == game->gameMap[i][j]) {
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(chessBoardMargin + gridSize * j - chessRadius, chessBoardMargin + gridSize * i - chessRadius,
                                    chessRadius * 2, chessRadius * 2);
                if (i == game->coordinate.rbegin()->getRowPos() && j == game->coordinate.rbegin()->getColPos()) {
                    QPen pen;
                    pen.setColor(Qt::red);
                    painter.setPen(pen);
                    painter.drawEllipse(chessBoardMargin + gridSize * j - chessRadius,
                                        chessBoardMargin + gridSize * i - chessRadius, markSizeAfter, markSizeAfter);
                }
            }

            if (-1 == game->gameMap[i][j]) {
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(chessBoardMargin + gridSize * j - chessRadius, chessBoardMargin + gridSize * i - chessRadius,
                                    chessRadius * 2, chessRadius * 2);
            }
        }
    }

    // 判断游戏进程是否结束
    // 判断游戏胜负
    if (clickRowPos > 0 && clickRowPos < chessBoardSize && clickColPos > 0 && clickColPos < chessBoardSize
        && (game->gameMap[clickRowPos][clickColPos] == 1 || game->gameMap[clickRowPos][clickColPos] == -1)) {
        Position currentPos(clickRowPos, clickColPos);
        if (game->winOrNot(currentPos) && game->gameStatus == ONGOING) {
            qDebug() << "胜利！";
            game->gameStatus = WIN;
            // QSound::play(WIN_SOUND);
            QString str;
            if (game->gameMap[clickRowPos][clickColPos] == 1)
                str = "黑子";
            else if (game->gameMap[clickRowPos][clickColPos] == -1)
                str = "白子";
            QMessageBox::StandardButton btnValue = QMessageBox::information(this, "恭喜", str + " 获胜");

            // 重置游戏状态，避免发生死循环
            if (btnValue == QMessageBox::Ok) {
                game->startGame(gameType_);
                game->gameStatus = ONGOING;
            }
        }
    }
    // 判断是否死局
    if (game->deadOrNot()) {
        QMessageBox::StandardButton btnValue = QMessageBox::information(this, "棋盘死局，", "本局游戏已结束");
        if (btnValue == QMessageBox::Ok) {
            game->startGame(gameType_);
            game->gameStatus = ONGOING;
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    // 通过鼠标的点击确定落子的标记
    int x = event->x();
    int y = event->y();

    // 棋盘边缘不能有棋子落子
    if (x >= chessBoardMargin + gridSize / 2 && x < size().width() - chessBoardMargin && y >= chessBoardMargin + gridSize / 2
        && y < size().height() - chessBoardMargin) {
        // 获取最近的左上角的点
        int col = x / gridSize;
        int row = y / gridSize;

        int leftTopPosX = chessBoardMargin + gridSize * col;
        int leftTopPosY = chessBoardMargin + gridSize * row;

        // 根据距离算出合适的点击位置,一共四个点，根据半径距离选最近的
        clickRowPos = -1;  // 初始化最终的值
        clickColPos = -1;
        int len = 0;       // 计算完后取整就可以了

        // 确定一个误差在范围内的点，且只可能确定一个出来
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < posRange) {
            clickRowPos = row;
            clickColPos = col;
        }
        len = sqrt((x - leftTopPosX - gridSize) * (x - leftTopPosX - gridSize) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < posRange) {
            clickRowPos = row;
            clickColPos = col + 1;
        }
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - gridSize) * (y - leftTopPosY - gridSize));
        if (len < posRange) {
            clickRowPos = row + 1;
            clickColPos = col;
        }
        len = sqrt((x - leftTopPosX - gridSize) * (x - leftTopPosX - gridSize)
                   + (y - leftTopPosY - gridSize) * (y - leftTopPosY - gridSize));
        if (len < posRange) {
            clickRowPos = row + 1;
            clickColPos = col + 1;
        }
    }

    // 存了坐标后也要重绘
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event) {
    // 人下棋，并且不能抢机器的棋
    if (!(gameType_ == PVE && !game->player)) {
        playOneChess_P();
        // 如果是人机对战模式，需要调用AI下棋
        if (game->gameType == PVE && !game->player) {
            // 用定时器做一个延迟
            QTimer::singleShot(aiChessDelay, this, SLOT(playOneChess_E()));
        }
    }
}

void MainWindow::playOneChess_P() {
    // 当且仅当鼠标点击位置有效并且该处无子的情况下才能落子
    if (clickRowPos != -1 && clickColPos != -1 && game->gameMap[clickRowPos][clickColPos] == 0) {
        Position tempPos(clickRowPos, clickColPos);
        game->act_P(tempPos);

        // 重绘页面
        update();
    }
}

void MainWindow::playOneChess_E() {
    if (clickRowPos != -1 && clickColPos != -1 && game->gameMap[clickRowPos][clickColPos] == 0) {
        Position tempPos(clickRowPos, clickColPos);
        game->act_E(tempPos);

        // 重绘页面
        update();
    }
}
