
#include "chessboard.h"
#include <QAction>
#include <QDebug>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include <QVBoxLayout>
#include <math.h>

const int cellSize         = 32;               // 棋盘网格边长
const int chessRadius      = cellSize * 0.45;  // 棋子半径
const int chessBoardMargin = 36;               // 页面边界距棋盘边缘空隙
const int posRange         = cellSize / 2;     // 鼠标点击的最大模糊边界距离
const int markSizeBefore   = 8;                // 预期落子位置标记的边长
const int markSizeAfter    = 4;                // 当前落子标记的边长
const int aiChessDelay     = 600;              // AI下棋前的思考时间

ChessBoard::ChessBoard(QWidget* parent) : QWidget{ parent } {
    setMouseTracking(true);
    setFixedSize(chessBoardMargin * 2 + cellSize * chessBoardSize,
                 chessBoardMargin * 2 + cellSize * chessBoardSize);
    game = new Game;
    initGame(PVP);
}

void ChessBoard::initGame(GameType type) {
    gameType         = type;
    game->gameStatus = OnGoing;
    game->startGame(gameType);
    update();
}

ChessBoard::~ChessBoard() {
    if (game)
        delete game;
}

void ChessBoard::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);
    // 绘制棋盘
    painter.setRenderHint(QPainter::Antialiasing, true);  // 设置窗口抗锯齿
    // 绘制棋盘上的线条
    QPen pen(QColor(0xCE, 0xD4, 0xDA));
    pen.setWidth(1);  // 设置绘制线条宽度
    painter.setPen(pen);
    for (int num = 0; num <= chessBoardSize; num++) {
        // 绘制棋盘网格竖线
        painter.drawLine(chessBoardMargin + cellSize * num, chessBoardMargin,  // 画线起点坐标
                         chessBoardMargin + cellSize * num,
                         size().height() - chessBoardMargin);  // 画线终点坐标
        // 绘制棋盘网格横线
        painter.drawLine(chessBoardMargin, chessBoardMargin + cellSize * num,
                         size().width() - chessBoardMargin, chessBoardMargin + cellSize * num);
    }

    // drow four points
    painter.setBrush(pen.color());
    int    dotRadius = 4;
    QPoint leftTop(chessBoardMargin + cellSize * chessBoardSize / 3 - dotRadius / 2,
                   chessBoardMargin + cellSize * chessBoardSize / 3 - dotRadius / 2);
    QPoint rightBottom(
        size().height() - chessBoardMargin - cellSize * chessBoardSize / 3 - dotRadius / 2,
        size().width() - chessBoardMargin - cellSize * chessBoardSize / 3 - dotRadius / 2);
    painter.drawEllipse(leftTop.x(), leftTop.y(), dotRadius, dotRadius);
    painter.drawEllipse(rightBottom.x(), leftTop.y(), dotRadius, dotRadius);
    painter.drawEllipse(leftTop.x(), rightBottom.y(), dotRadius, dotRadius);
    painter.drawEllipse(rightBottom.x(), rightBottom.y(), dotRadius, dotRadius);

    // 填充棋盘以及棋子的颜色
    QBrush brush;
    QColor whiteChessColor(0x18, 0xBC, 0x9C), blackChessColor(0x2C, 0x3E, 0x50);
    brush.setStyle(Qt::SolidPattern);  // 设置颜色填充的模式

    // 绘制预期棋子落子位置标记
    if (clickRowPos > 0 && clickRowPos < chessBoardSize && clickColPos > 0
        && clickColPos < chessBoardSize && game->gameMap[clickRowPos][clickColPos] == 0) {
        if (game->player)
            brush.setColor(blackChessColor);
        else
            brush.setColor(whiteChessColor);

        painter.setPen(pen);
        painter.drawRect(chessBoardMargin + cellSize * clickColPos - markSizeBefore / 2,
                         chessBoardMargin + cellSize * clickRowPos - markSizeBefore / 2,
                         markSizeBefore, markSizeBefore);
    }

    // 绘制棋子
    for (int i = 0; i < chessBoardSize; i++) {
        for (int j = 0; j < chessBoardSize; j++) {
            //绘制黑棋
            if (game->gameMap[i][j] != 0) {
                switch (game->gameMap[i][j]) {
                case 1:
                    brush.setColor(whiteChessColor);
                    break;
                case -1:
                    brush.setColor(blackChessColor);
                    break;
                }
                painter.setBrush(brush);
                painter.drawEllipse(chessBoardMargin + cellSize * j - chessRadius,
                                    chessBoardMargin + cellSize * i - chessRadius, chessRadius * 2,
                                    chessRadius * 2);
                //绘制当前棋子落子后标记
                if (i == game->coordinate.rbegin()->getRowPos()
                    && j == game->coordinate.rbegin()->getColPos()) {
                    painter.setBrush(Qt::white);
                    painter.drawEllipse(chessBoardMargin + cellSize * j - markSizeAfter / 2,
                                        chessBoardMargin + cellSize * i - markSizeAfter / 2,
                                        markSizeAfter, markSizeAfter);
                }
            }
        }
    }

    // 判断游戏进程是否结束
    // 判断游戏胜负
    if (clickRowPos > 0 && clickRowPos < chessBoardSize && clickColPos > 0
        && clickColPos < chessBoardSize
        && (game->gameMap[clickRowPos][clickColPos] == 1
            || game->gameMap[clickRowPos][clickColPos] == -1)) {
        Position currentPos(clickRowPos, clickColPos);
        if (game->isGameOver(currentPos) && game->gameStatus == OnGoing) {
            emit gameOver();
        }
    }
    // 判断是否死局
    if (game->deadOrNot()) {
        QMessageBox::StandardButton btnValue =
            QMessageBox::information(this, "棋盘死局，", "本局游戏已结束");
        if (btnValue == QMessageBox::Ok) {
            game->startGame(gameType);
            game->gameStatus = OnGoing;
        }
    }
}

void ChessBoard::mouseMoveEvent(QMouseEvent* event) {
    // 通过鼠标的点击确定落子的标记
    int x = event->x();
    int y = event->y();

    // 棋盘边缘不能有棋子落子
    if (x >= chessBoardMargin + cellSize / 2 && x < size().width() - chessBoardMargin
        && y >= chessBoardMargin + cellSize / 2 && y < size().height() - chessBoardMargin) {
        // 获取最近的左上角的点
        int col = x / cellSize;
        int row = y / cellSize;

        int leftTopPosX = chessBoardMargin + cellSize * col;
        int leftTopPosY = chessBoardMargin + cellSize * row;

        // 根据距离算出合适的点击位置,一共四个点，根据半径距离选最近的
        clickRowPos = -1;  // 初始化最终的值
        clickColPos = -1;
        int len     = 0;  // 计算完后取整就可以了

        // 确定一个误差在范围内的点，且只可能确定一个出来
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < posRange) {
            clickRowPos = row;
            clickColPos = col;
        }
        len = sqrt((x - leftTopPosX - cellSize) * (x - leftTopPosX - cellSize)
                   + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < posRange) {
            clickRowPos = row;
            clickColPos = col + 1;
        }
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX)
                   + (y - leftTopPosY - cellSize) * (y - leftTopPosY - cellSize));
        if (len < posRange) {
            clickRowPos = row + 1;
            clickColPos = col;
        }
        len = sqrt((x - leftTopPosX - cellSize) * (x - leftTopPosX - cellSize)
                   + (y - leftTopPosY - cellSize) * (y - leftTopPosY - cellSize));
        if (len < posRange) {
            clickRowPos = row + 1;
            clickColPos = col + 1;
        }
    }

    // 存了坐标后也要重绘
    update();
}

void ChessBoard::mouseReleaseEvent(QMouseEvent* event) {
    // 人下棋，并且不能抢机器的棋
    if (!(gameType == PVE && !game->player)) {
        playOneChess_P();
        // 如果是人机对战模式，需要调用AI下棋
        if (game->gameType == PVE && !game->player) {
            // 用定时器做一个延迟
            QTimer::singleShot(aiChessDelay, this, SLOT(playOneChess_E()));
        }
        emit makeMove();
    }
}

void ChessBoard::playOneChess_P() {
    // 当且仅当鼠标点击位置有效并且该处无子的情况下才能落子
    if (clickRowPos != -1 && clickColPos != -1 && game->gameMap[clickRowPos][clickColPos] == 0) {
        Position tempPos(clickRowPos, clickColPos);
        game->act_P(tempPos);

        // 重绘页面
        update();
    }
}

void ChessBoard::playOneChess_E() {
    if (clickRowPos != -1 && clickColPos != -1 && game->gameMap[clickRowPos][clickColPos] == 0) {
        Position tempPos(clickRowPos, clickColPos);
        game->act_E(tempPos);

        // 重绘页面
        update();
    }
}
