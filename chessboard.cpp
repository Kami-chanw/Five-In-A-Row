
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

const int    ChessBoard::cellSize         = 32;               // 棋盘网格边长
const int    ChessBoard::chessRadius      = cellSize * 0.42;  // 棋子半径
const int    ChessBoard::chessBoardMargin = 24;               // 页面边界距棋盘边缘空隙
const QRect  ChessBoard::clickBoundingBox(-ChessBoard::chessRadius, -ChessBoard::chessRadius,
                                          ChessBoard::chessRadius * 2 - 1,
                                          ChessBoard::chessRadius * 2 - 1);  // 鼠标点击的判定框
const int    ChessBoard::aiChessDelay      = 600;  // AI下棋前的思考时间
const int    ChessBoard::markSizeAfterPlay = 4;    // 落子标记大小
const QColor ChessBoard::whiteChessColor(0x18, 0xBC, 0x9C);
const QColor ChessBoard::blackChessColor(0x2C, 0x3E, 0x50);

ChessBoard::ChessBoard(GameType type, QWidget* parent) : QWidget{ parent } {
    setMouseTracking(true);
    setFixedSize(chessBoardMargin * 2 + cellSize * (chessBoardSize - 1),
                 chessBoardMargin * 2 + cellSize * (chessBoardSize - 1));
    game = new Game(type);
}

void ChessBoard::initGame() {
    game->setGameStatus(Status::OnGoing);
    game->startGame();
}

ChessBoard::~ChessBoard() {
    if (game)
        delete game;
}

void ChessBoard::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);

    // draw background
    painter.fillRect(rect(), Qt::white);

    // draw grid lines
    QPen pen(QColor(0xCE, 0xD4, 0xDA));
    pen.setWidth(1);
    painter.setPen(pen);
    for (int i = 0; i < chessBoardSize; i++) {
        painter.drawLine(chessBoardMargin + cellSize * i, chessBoardMargin,
                         chessBoardMargin + cellSize * i, height() - chessBoardMargin);
        painter.drawLine(chessBoardMargin, chessBoardMargin + cellSize * i,
                         width() - chessBoardMargin, chessBoardMargin + cellSize * i);
    }
    painter.setPen(Qt::NoPen);

    // drow locate points
    painter.setBrush(pen.color());
    int    dotRadius = 6;
    QPoint leftTop(chessBoardMargin + cellSize * (chessBoardSize / 3 - 1) - dotRadius / 2,
                   chessBoardMargin + cellSize * (chessBoardSize / 3 - 1) - dotRadius / 2);
    QPoint rightBottom(
        size().height() - chessBoardMargin - cellSize * (chessBoardSize / 3 - 1) - dotRadius / 2,
        size().width() - chessBoardMargin - cellSize * (chessBoardSize / 3 - 1) - dotRadius / 2);
    painter.drawEllipse(leftTop.x(), leftTop.y(), dotRadius, dotRadius);
    painter.drawEllipse(rightBottom.x(), leftTop.y(), dotRadius, dotRadius);
    painter.drawEllipse(leftTop.x(), rightBottom.y(), dotRadius, dotRadius);
    painter.drawEllipse(rightBottom.x(), rightBottom.y(), dotRadius, dotRadius);
    painter.drawEllipse(chessBoardMargin + cellSize * (chessBoardSize / 2) - dotRadius / 2,
                        chessBoardMargin + cellSize * (chessBoardSize / 2) - dotRadius / 2,
                        dotRadius, dotRadius);

    // draw chess before moving
    if (clickPos.isValid(chessBoardSize) && game->isEmptyAt(clickPos)) {
        painter.setBrush(game->isBlackPlay() ? blackChessColor : whiteChessColor);
        painter.drawEllipse(chessBoardMargin + cellSize * clickPos.col() - chessRadius,
                            chessBoardMargin + cellSize * clickPos.row() - chessRadius,
                            chessRadius * 2, chessRadius * 2);
    }

    // paint move
    for (int i = 0; i < chessBoardSize; i++) {
        for (int j = 0; j < chessBoardSize; j++) {
            // draw chess
            if (!game->isEmptyAt(i, j)) {
                painter.setBrush(game->isWhiteAt(i, j) ? whiteChessColor : blackChessColor);
                painter.drawEllipse(chessBoardMargin + cellSize * j - chessRadius,
                                    chessBoardMargin + cellSize * i - chessRadius, chessRadius * 2,
                                    chessRadius * 2);
                // draw mark after moving
                if (game->getLastPos() == Position{ i, j }) {
                    painter.setBrush(Qt::white);
                    painter.drawEllipse(chessBoardMargin + cellSize * j - markSizeAfterPlay / 2,
                                        chessBoardMargin + cellSize * i - markSizeAfterPlay / 2,
                                        markSizeAfterPlay, markSizeAfterPlay);
                }
            }
        }
    }

    if (game->getGameStatus() == Status::Win) {
        QPixmap  winFlag = QPixmap(":/imgs/mainwnd_chessstar.svg");
        QPainter p(&winFlag);
        p.setCompositionMode(QPainter::CompositionMode_SourceIn);
        p.setBrush(game->isBlackPlay() ? blackChessColor : whiteChessColor);
        p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);
        p.drawRect(winFlag.rect());
        for (const auto& pos : winPos) {
            QRect winFlagRect(0, 0, chessRadius, chessRadius);
            winFlagRect.moveCenter({ chessBoardMargin + pos.col() * cellSize - 1,
                                     chessBoardMargin + pos.row() * cellSize - 1 });
            painter.drawPixmap(winFlagRect, winFlag);
        }
    }
}

void ChessBoard::mouseMoveEvent(QMouseEvent* event) {
    QPoint pos = event->pos();
    clickPos.setRow(-1);
    clickPos.setCol(-1);

    QRect boundingBox = clickBoundingBox.translated(pos);

    Position mousePos((pos.y() - chessBoardMargin) / cellSize,
                      (pos.x() - chessBoardMargin) / cellSize);
    int      mouseColX = mousePos.col() * cellSize + chessBoardMargin;
    int      mouseRowY = mousePos.row() * cellSize + chessBoardMargin;

    if (boundingBox.contains({ mouseColX, mouseRowY }))
        clickPos = mousePos;
    else if (boundingBox.contains({ mouseColX + cellSize, mouseRowY }))
        clickPos = { mousePos.row(), mousePos.col() + 1 };
    else if (boundingBox.contains({ mouseColX, mouseRowY + cellSize }))
        clickPos = { mousePos.row() + 1, mousePos.col() };
    else if (boundingBox.contains({ mouseColX + cellSize, mouseRowY + cellSize }))
        clickPos = { mousePos.row() + 1, mousePos.col() + 1 };

    update();
}

void ChessBoard::mouseReleaseEvent(QMouseEvent* event) {
    if (!(game->getGameType() == GameType::PVE && !game->isBlackPlay())) {
        playOneChess_P();
        // 如果是人机对战模式，需要调用AI下棋
        if (game->getGameType() == GameType::PVE && !game->isBlackPlay()) {
            // 用定时器做一个延迟
            QTimer::singleShot(aiChessDelay, this, &ChessBoard::playOneChess_E);
        }
    }
}

void ChessBoard::playOneChess_P() {
    // 当且仅当鼠标点击位置有效并且该处无子的情况下才能落子
    if (clickPos.isValid(chessBoardSize) && game->isEmptyAt(clickPos)) {
        game->act_P(clickPos);
        auto&& [isWin, winPos] = game->isWin();
        emit makeMove();
        if (isWin) {
            this->winPos = winPos;
            emit gameOver(true);
        }
        else if (game->isDead()) {
            emit gameOver(false);
        }
        update();
    }
}

void ChessBoard::playOneChess_E() {
    game->act_E();
    auto&& [isWin, winPos] = game->isWin();
    emit makeMove();
    update();
    if (isWin) {
        this->winPos = winPos;
        emit gameOver(true);
    }
    else if (game->isDead()) {
        emit gameOver(false);
    }
}
