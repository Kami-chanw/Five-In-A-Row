
#ifndef CHESSBOARDWIDGET_H
#define CHESSBOARDWIDGET_H


#include <QWidget>
#include"Game.h"


class ChessBoardWidget : public QWidget
{
    Q_OBJECT
public:
    ChessBoardWidget(GameType=PVP,QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);//绘图
    void mouseMoveEvent(QMouseEvent *event);//监测鼠标移动事件
    void mouseReleaseEvent(QMouseEvent *event);//监测鼠标释放事件，获取棋子落子位置


    Game *game;//存储游戏指针
    GameType gameType_;//存储游戏类型
    int clickRowPos,clickColPos;//存储鼠标点击的位置坐标

    void initGame();//初始化游戏
    void checkGame();//查询游戏进程状态

private slots:
    //初始化双人对战模式和人机对战模式
    void initPVPGame();
    void initPVEGame();

    //人落子和AI落子
    void playOneChess_P();
    void playOneChess_E();


};

#endif // CHESSBOARDWIDGET_H
