
#ifndef FIVECHESSL_H
#define FIVECHESS_H
#include<vector>
using namespace std;
enum Condition
{
    Play,
    Win,
    Lose
};
enum GameModel
{
    PERSON,
    AI
};
enum Difficulty
{

};

const int chessboardSize=15;//还是可自定义，待定
class FiveChess
{
public:
    FiveChess(GameModel,Difficulty);
    ~FiveChess();
   //数据成员
    //Position：一种vector<vector<int>>Map;另一种struct Position{}; vector<Position>Map;
    vector<vector<int>>Map;//空白为0，黑子1，白子-1
    //游戏模式，游戏状态,游戏难度
    GameModel model;
    Condition condition;
    Difficulty difficulty;
    //标示下棋方
    bool playerFlag;

   //接口函数：

    //获取游戏模式-> 通过构造函数获取
    //void setModel();
    //获取选择难度 -> 通过构造函数获取
    //void setDifficulty();

    //开始游戏 -> 创建棋盘对象即开始游戏

    //接受UI下棋信号和下棋方：获取光标点击信号
    //下棋，更新棋盘
    void setChess(int,int);
    //判断游戏是否结束:横、竖、左斜、右斜四个方向
    bool isWin(int,int);
    //判断是否和棋
    bool isDead();


};

#endif
