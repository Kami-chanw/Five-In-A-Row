
#ifndef GAME_H
#define GAME_H
#include"Position.h"
#include<vector>

const int  chessBoardSize=15;//棋盘尺寸为15*15；
const int  maxNumber= 225;//棋盘上最多存在225个棋子；

//游戏类型：人机对战、双人对战（本地）
enum GameType{PVP,PVE};

//游戏状态：进行中、已结束（胜负或死局）
enum Status{ONGOING,WIN,DEAD};

enum Piece{BLACK=-1,WHITE=1};


class Game
{
public:
    Game();
    void startGame(GameType type);//对游戏进行初始化

    void updateGameMap(Position pos);//根据最新落子更新棋盘上棋子分布情况
    void clearGameMap();//清空棋盘上分布的所有棋子；

    void act_P(Position pos);//人类下棋
    void act_E(Position pos);//AI下棋

    bool winOrNot(Position pos);//判断游戏是否胜利
    bool deadOrNot();//判断是否游戏已经死局


    GameType gameType;//游戏类型
    Status gameStatus;//游戏状态
    bool player;//表示下棋方（黑子-1或者白子1)
    std::vector<std::vector<int>>gameMap;//存储棋盘以及棋盘上棋子的分布状况
    std::vector<Position>coordinate;//存储已经落子的棋子坐标

};

#endif // GAME_H
