#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"Game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  protected:
  void paintEvent(QPaintEvent *event);//绘图
  void mouseMoveEvent(QMouseEvent *event);//监测鼠标移动事件
  void mouseReleaseEvent(QMouseEvent *event);//监测鼠标释放事件，获取棋子落子位置

private:
  Ui::MainWindow *ui;
  Game *game;//存储游戏指针
  GameType gameType_;//存储游戏类型
  int clickRowPos,clickColPos;//存储鼠标点击的位置坐标

  bool determineBoundaries(int crow,int ccol);
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
#endif // MAINWINDOW_H
