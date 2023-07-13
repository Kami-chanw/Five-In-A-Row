
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

ChessBoard::ChessBoard(GameType type, QWidget* parent) : QWidget{ parent } {
    setMouseTracking(true);
    setFixedSize(chessBoardMargin * 2 + cellSize * (chessBoardSize - 1),
                 chessBoardMargin * 2 + cellSize * (chessBoardSize - 1));
}

void ChessBoard::initGame() {
    game.setGameStatus(Game::OnGoing);
    game.startGame();
}

ChessBoard::~ChessBoard() {}

void ChessBoard::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);

    // draw background
    painter.fillRect(rect(), Qt::white);

    // draw grid lines
    painter.setPen(QColor(0xCE, 0xD4, 0xDA));
    for (int i = 0; i < chessBoardSize; i++) {
        painter.drawLine(chessBoardMargin + cellSize * i, chessBoardMargin,
                         chessBoardMargin + cellSize * i, height() - chessBoardMargin);
        painter.drawLine(chessBoardMargin, chessBoardMargin + cellSize * i,
                         width() - chessBoardMargin, chessBoardMargin + cellSize * i);
    }

    // drow locate points
    painter.setBrush(painter.pen().color());
    int    dotRadius = 4;
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
    painter.setPen(Qt::NoPen);

    // draw chess before moving
    if (clickPos.isValid(chessBoardSize) && game.isEmptyAt(clickPos)) {
        painter.setBrush(game.isBlackPlay() ? blackChessColor : whiteChessColor);
        painter.drawEllipse(chessBoardMargin + cellSize * clickPos.col() - chessRadius,
                            chessBoardMargin + cellSize * clickPos.row() - chessRadius,
                            chessRadius * 2, chessRadius * 2);
    }

    // paint move
    for (int i = 0; i < chessBoardSize; i++) {
        for (int j = 0; j < chessBoardSize; j++) {
            // draw chess
            if (!game.isEmptyAt(i, j)) {
                painter.setBrush(game.isWhiteAt(i, j) ? whiteChessColor : blackChessColor);
                painter.drawEllipse(chessBoardMargin + cellSize * j - chessRadius,
                                    chessBoardMargin + cellSize * i - chessRadius, chessRadius * 2,
                                    chessRadius * 2);
                // draw mark after moving
                if (game.getLastPos() == Position{ i, j }) {
                    painter.setBrush(Qt::white);
                    painter.drawEllipse(chessBoardMargin + cellSize * j - markSizeAfterPlay / 2,
                                        chessBoardMargin + cellSize * i - markSizeAfterPlay / 2,
                                        markSizeAfterPlay, markSizeAfterPlay);
                }
            }
        }
    }

    if (game.getGameStatus() == Game::Win) {
        QPixmap  winFlag = QPixmap(":/imgs/mainwnd_chessstar.svg");
        QPainter p(&winFlag);
        p.setCompositionMode(QPainter::CompositionMode_SourceIn);
        p.setBrush(game.isBlackPlay() ? blackChessColor : whiteChessColor);
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
    if (!(currGameType == GameType::PVE && !game.isBlackPlay())) {
        makePlayerMove();
        // 如果是人机对战模式，需要调用AI下棋
        if (currGameType == GameType::PVE && !game.isBlackPlay()) {
            // 用定时器做一个延迟
            QTimer::singleShot(aiChessDelay, this, &ChessBoard::makeAiMove);
        }
    }
}

void ChessBoard::makePlayerMove() {
    // 当且仅当鼠标点击位置有效并且该处无子的情况下才能落子
    if (clickPos.isValid(chessBoardSize) && game.isEmptyAt(clickPos)) {
        game.makePlayerMove(clickPos);
        auto&& [isWin, winPos] = game.isWin();
        emit makeMove();
        if (isWin) {
            this->winPos = winPos;
            emit gameOver(true);
        }
        else if (game.isDead()) {
            emit gameOver(false);
        }
        update();
    }
}

void ChessBoard::makeAiMove() {
    game.makeAiMove();
    auto&& [isWin, winPos] = game.isWin();
    emit makeMove();
    update();
    if (isWin) {
        this->winPos = winPos;
        emit gameOver(true);
    }
    else if (game.isDead()) {
        emit gameOver(false);
    }
}

GameType ChessBoard::gameType() const { return currGameType; }

void ChessBoard::setGameType(GameType newCurrGameType) { currGameType = newCurrGameType; }
