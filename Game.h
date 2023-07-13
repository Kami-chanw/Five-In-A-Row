#ifndef GAME_H
#define GAME_H
#include "position.h"
#include <array>
#include <vector>

const int chessBoardSize = 15;
const int maxNumber      = chessBoardSize * chessBoardSize;

class Game {
public:
    enum Status { OnGoing, Win, Dead };
    enum Piece {
        Empty = 0,
        White = 1,
        Black = 2,
    };
    Game();
    ~Game();
    void startGame();
    void updateGameMap(Position);

    bool isBlackPlay() const { return player; }

    Piece chessAt(int row, int col) { return gameMap[row][col]; }
    Piece chessAt(Position pos) { return gameMap[pos.row()][pos.col()]; }
    void  setChessAt(Position pos, Piece chess) { gameMap[pos.row()][pos.col()] = chess; }
    void  makePlayerMove(Position pos);
    void  makeAiMove();

    void   setGameStatus(Status status) { gameStatus = status; }
    Status getGameStatus() const { return gameStatus; }

    Position getLastPos();
    bool     isEmptyAt(Position pos) { return chessAt(pos) == Piece::Empty; }
    bool     isWhiteAt(Position pos) { return chessAt(pos) == Piece::White; }
    bool     isBlackAt(Position pos) { return chessAt(pos) == Piece::Black; }
    bool     isEmptyAt(int row, int col) { return chessAt(row, col) == Piece::Empty; }
    bool     isWhiteAt(int row, int col) { return chessAt(row, col) == Piece::White; }
    bool     isBlackAt(int row, int col) { return chessAt(row, col) == Piece::Black; }

    std::pair<bool, std::array<Position, 5>> isWin();

    bool isDead();

private:
    std::vector<Position>           history;
    std::vector<std::vector<Piece>> gameMap;

    std::vector<std::vector<int>> scoreMap;

    Status gameStatus;
    bool   player = true;

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
    void     calculateScore();
    Position getBestChess();
};

#endif
