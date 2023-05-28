
#include "Game.h"

Game::Game() {
}

void Game::startGame(GameType type) {
    gameType = type;  // 选定游戏类型
    gameMap.clear();
    clearGameMap();
    player = true;
}

void Game::clearGameMap() {
    for (int i = 0; i < chessBoardSize; i++) {

        std::vector<int>lineChessBoard;

        for (int j = 0; j < chessBoardSize; j++) {
            lineChessBoard.push_back(0);
        }
        gameMap.push_back(lineChessBoard);
    }
}

void Game::updateGameMap(Position pos) {
    coordinate.push_back(pos);

    // 根据player情况落子到棋盘上，用正负1区分棋子颜色，-1为黑子，1为白子
    if (true == player)
        gameMap[pos.getRowPos()][pos.getColPos()] = -1;
    else
        gameMap[pos.getRowPos()][pos.getColPos()] = 1;

    // 完成上一次落子后，换手准备进行下一次落子
    player = !player;
}

void Game::act_P(Position pos) {
    updateGameMap(pos);
}

void Game::act_E(Position pos) {

    updateGameMap(pos);
}
bool Game::winOrNot(Position pos) {
    if (coordinate.size()==10)
    {
        return true;
    }

}

bool Game::deadOrNot() {
    if (maxNumber == coordinate.size())
        return true;
    return false;
}
