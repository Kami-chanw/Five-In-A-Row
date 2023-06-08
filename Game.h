#ifndef GAME_H
#define GAME_H
#include "ACSearcher.h"
#include "Position.h"
#include "PossiblePositionManager.h"
#include <cstring>
#include <stack>
#include <string>
#include <vector>
#define UNKNOWN_SCORE (10000001)
#define HASH_ITEM_INDEX_MASK (0xffff)
#define MAX_SCORE (10000000)
#define MIN_SCORE (-10000000)
enum class Status { ONGOING, WIN, DEAD };
enum class GameType { PVP, PVE };

/*enum Piece
{
    WHITE=1,
    BLACK=-1,
};*/
enum Role { HUMAN = 1, COMPUTOR = 2, EMPTY = 0 };

/*struct Pos{
    int row;
    int col;
    int chess;
};*/
//根据ui接口改用Position类
const int chessboardSize = 15;
const int maxNumber      = 225;
class Game {
private:
    //数据成员
    // Position：一种vector<vector<int>>Map;另一种struct Position{}; vector<Position>Map;
    std::vector<Position>         coordinate;  //保存棋盘情况
    std::vector<std::vector<int>> gameMap;     //棋盘上棋子分布情况

    std::vector<std::vector<int>> scoreMap;  //记录棋盘上各点的评分

    // vector<vector<int>> Map;
    //游戏模式，游戏状态,游戏难度
    GameType gameType;
    Status   gameStatus;
    // Difficulty difficulty
    bool player;  //标示下棋方

    void calculateScore();  //评分计算（不同棋形对应不同评分），便于AI根据评分思考
    Position getBestChess();  //评分中找到最大分数位置

    int       DEPTH = 7;
    int       scores[2][72];  //保存棋局分数（2个角色72行，包括横竖撇捺）
    int       allScore[2];    //局面总评分（2个角色）
    char      board[chessboardSize][chessboardSize];
    long long boardZobristValue[2][chessboardSize][chessboardSize];
    long long currentZobristValue;
    int       winner;  //胜出者
    //保存棋局的哈希表条目
    struct HashItem {
        long long checksum;
        int       depth;
        int       score;
        enum Flag { ALPHA = 0, BETA = 1, EXACT = 2, EMPTY = 3 } flag;
    };

    HashItem hashItems[HASH_ITEM_INDEX_MASK + 1];
    int      getHashItemScore(int depth, int alpha, int beta);
    //存储搜索结果，即下一步棋子的位置
    Position searchResult;
    // ac算法实现的模式匹配器
    ACSearcher              acs;
    PossiblePositionManager ppm;

public:
    void recordHashItem(int depth, int score, HashItem::Flag flag);
    void randomBoardZobristValue();
    void initCurrentZobristValue();
    int  evaluatePoint(char board[chessboardSize][chessboardSize], Position p);
    int  evaluate(char board[chessboardSize][chessboardSize], Role role);
    void updateScore(char board[chessboardSize][chessboardSize], Position p);
    int  abSearch(char board[chessboardSize][chessboardSize], int depth, int alpha, int beta,
                  Role currentSearchRole);

    //接口函数：
    Game();
    ~Game();
    //开始游戏,初始化，获取游戏模式
    void startGame(GameType);
    // void setDifficulty();
    //接受UI下棋信号和下棋方：获取光标点击信号
    //下棋，更新棋盘
    void updateGameMap(Position);
    void clearGameMap();  //清空棋盘上所有棋子,初始化棋盘

    void act_P(Position pos);  //人下棋
    void act_E(Position pos);  // AI下棋

    Position getLastPos();          //获取最后一个棋子信息
    bool     EmptyOrNot(Position);  //判断棋盘上此位置是否空
    bool     WinOrNot();  //判断游戏是否结束:横、竖、左斜、右斜四个方向
    //悔棋操作??待实现
    bool DeadOrNot();  //判断是否和棋
};

#endif
