#ifndef GAME_H
#define GAME_H
#include "Position.h"
#include <vector>
enum class Status { OnGoing, Win, Dead };
enum class GameType { PVP, PVE };
/* enum Difficulty {

};*/
enum class Piece {
    Empty = 0,
    White = 1,
    Black = -1,
};

/*struct Pos{
    int row;
    int col;
    int chess;
};*/
// 根据ui接口改用Position类
const int chessBoardSize = 15;
const int maxNumber      = 225;
class Game {
private:
    // 数据成员
    //  Position：一种vector<vector<int>>Map;另一种struct Position{}; vector<Position>Map;
    std::vector<Position>           coordinate;  // 保存棋盘情况
    std::vector<std::vector<Piece>> gameMap;     // 棋盘上棋子分布情况

    std::vector<std::vector<int>> scoreMap;  // 记录棋盘上各点的评分

    // vector<vector<int>> Map;//空白为0，白子1，黑子-1
    // 游戏模式，游戏状态,游戏难度
    GameType gameType;
    Status   gameStatus;
    // Difficulty difficulty
    bool player = true;  // 标示下棋方

    template <class T, class... Ts>
    void clearMap(std::vector<std::vector<T>>& m, Ts&... args) {
        std::vector<T> tmp;
        if constexpr (std::is_same_v<T, int>)
            tmp.assign(chessBoardSize, 0);
        else
            tmp.assign(chessBoardSize, Piece::Empty);
        m.assign(chessBoardSize, tmp);
        if constexpr (sizeof...(args)) {
            clearMap(args...);
        }
    }
    void calculateScore();  // 评分计算（不同棋形对应不同评分），便于AI根据评分思考
    Position getBestChess();  // 评分中找到最大分数位置
public:                       // 接口函数：
    Game();
    ~Game();
    // 开始游戏,初始化，获取游戏模式
    void startGame(GameType);
    // void setDifficulty();
    // 接受UI下棋信号和下棋方：获取光标点击信号
    // 下棋，更新棋盘
    void updateGameMap(Position);

    bool isBlackPlay() const { return player; }

    Piece chessAt(int row, int col) { return gameMap[row][col]; }
    Piece chessAt(Position pos) { return gameMap[pos.getRowPos()][pos.getColPos()]; }
    void  setChessAt(Position pos, Piece chess) {
         gameMap[pos.getRowPos()][pos.getColPos()] = chess;
    }
    void act_P(Position pos);  // 人下棋
    void act_E();  // AI下棋

    void     setGameStatus(Status status) { gameStatus = status; }
    Status   getGameStatus() const { return gameStatus; }
    void     setGameType(GameType type) { gameType = type; }
    GameType getGameType() const { return gameType; }

    Position getLastPos();  // 获取最后一个棋子信息
    bool     isEmptyAt(Position pos) { return chessAt(pos) == Piece::Empty; }
    bool     isWhiteAt(Position pos) { return chessAt(pos) == Piece::White; }
    bool     isBlackAt(Position pos) { return chessAt(pos) == Piece::Black; }
    bool     isEmptyAt(int row, int col) { return chessAt(row, col) == Piece::Empty; }
    bool     isWhiteAt(int row, int col) { return chessAt(row, col) == Piece::White; }
    bool     isBlackAt(int row, int col) { return chessAt(row, col) == Piece::Black; }

    bool isWin();  // 判断游戏是否结束:横、竖、左斜、右斜四个方向
    // 悔棋操作??待实现
    bool isDead();  // 判断是否和棋
};

#endif
