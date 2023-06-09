
#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "Game.h"
#include <QWidget>

class ChessBoard : public QWidget {
    Q_OBJECT
public:
    ChessBoard(QWidget* parent = nullptr);
    void paintEvent(QPaintEvent* event);         //绘图
    void mouseMoveEvent(QMouseEvent* event);     //监测鼠标移动事件
    void mouseReleaseEvent(QMouseEvent* event);  //监测鼠标释放事件，获取棋子落子位置

    void     initGame(GameType);                                //初始化游戏
    GameType gameType() const { return game->getGameType(); };  //查询游戏进程状态

    ~ChessBoard();

signals:
    void makeMove();
    void gameOver(bool isWin);

private slots:

    //人落子和AI落子
    void playOneChess_P();
    void playOneChess_E();

private:
    Game* game = nullptr;            //存储游戏指针
    int   clickRowPos, clickColPos;  //存储鼠标点击的位置坐标
};

#endif  // CHESSBOARD_H
