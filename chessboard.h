#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "game.h"
#include "position.h"
#include <QWidget>

namespace MetaTypes {
    Q_NAMESPACE
    enum GameType { PVP, PVE };
    Q_ENUM_NS(GameType)
}  // namespace MetaTypes
using MetaTypes::GameType;

class ChessBoard : public QWidget {
    Q_OBJECT
public:
    ChessBoard(GameType type, QWidget* parent = nullptr);
    void paintEvent(QPaintEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    void initGame();

    ~ChessBoard();

    GameType gameType() const;
    void     setGameType(GameType newCurrGameType);

signals:
    void makeMove();
    void gameOver(bool isWin);

private slots:

    void makePlayerMove();
    void makeAiMove();

private:
    Game                    game;
    Position                clickPos;  // click position mapping to chess board
    GameType                currGameType;
    std::array<Position, 5> winPos;  // the recent win position for 5 chesses

    static constexpr QColor blackChessColor  = QColor(0x2C, 0x3E, 0x50);
    static constexpr QColor whiteChessColor  = QColor(0x18, 0xBC, 0x9C);
    static constexpr int    cellSize         = 32;               // size of each cell in grid
    static constexpr int    chessRadius      = cellSize * 0.42;  // radius of chesses
    static constexpr int    chessBoardMargin = 24;  // margin between border and chess board
    static constexpr QRect  clickBoundingBox =
        QRect(-ChessBoard::chessRadius, -ChessBoard::chessRadius, ChessBoard::chessRadius * 2 - 1,
              ChessBoard::chessRadius * 2 - 1);  // bounding box of click
    static constexpr int markSizeAfterPlay = 4;  // mark size on chess after play
    static constexpr int aiChessDelay      = 600;
};

#endif  // CHESSBOARD_H
