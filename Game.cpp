#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <vector>
Game::Game() { srand(time(0)); }
Game::~Game() {}

void Game::startGame(GameType type) {
    // 获取游戏模式
    gameType = type;
    // this->difficulty=difficulty;
    // 初始化棋盘
    clearMap(gameMap, scoreMap);
}

void Game::updateGameMap(Position pos) {

    if (isEmptyAt(pos))  // 判断是否为空
    {
        if (player) {
            setChessAt(pos, Piece::Black);
        }
        else {
            setChessAt(pos, Piece::White);
        }
        player = !player;
        coordinate.push_back(pos);
    }
}

void Game::act_P(Position pos) { updateGameMap(pos); }
void Game::act_E() { updateGameMap(getBestChess()); }

Position Game::getLastPos() { return coordinate.back(); }
bool     Game::isWin() {
        int lastrow = getLastPos().getRowPos();
        int lastcol = getLastPos().getColPos();
        // 原理：从刚才落子的位置判断，判断其4个方向上是否有五颗连续的同类棋子
        // 水平方向:左0右+4，左-1右+3，左-2右+2，左-3右+1，左-4右0

        for (int i = 0; i < 5; i++) {
            if (lastcol - i >= 0 && lastcol - i + 4 <= chessBoardSize
            && gameMap[lastrow][lastcol - i] == gameMap[lastrow][lastcol - i + 1]
            && gameMap[lastrow][lastcol - i] == gameMap[lastrow][lastcol - i + 2]
            && gameMap[lastrow][lastcol - i] == gameMap[lastrow][lastcol - i + 3]
            && gameMap[lastrow][lastcol - i] == gameMap[lastrow][lastcol - i + 4]) {
                gameStatus = Status::Win;
                return true;
        }
    }
        // 竖直方向
        for (int i = 0; i < 5; i++) {
            if (lastrow - i >= 0 && lastrow - i + 4 <= chessBoardSize
            && gameMap[lastrow - i][lastcol] == gameMap[lastrow - i + 1][lastcol]
            && gameMap[lastrow - i][lastcol] == gameMap[lastrow - i + 2][lastcol]
            && gameMap[lastrow - i][lastcol] == gameMap[lastrow - i + 3][lastcol]
            && gameMap[lastrow - i][lastcol] == gameMap[lastrow - i + 4][lastcol]) {
                gameStatus = Status::Win;
                return true;
        }
    }
        //  /方向
        for (int i = 0; i < 5; i++) {
            if (lastrow + i < chessBoardSize && lastrow + i - 4 >= 0 && lastcol - i >= 0
            && lastcol - i + 4 < chessBoardSize
            && gameMap[lastrow + i][lastcol - i] == gameMap[lastrow + i - 1][lastcol - i + 1]
            && gameMap[lastrow + i][lastcol - i] == gameMap[lastrow + i - 2][lastcol - i + 2]
            && gameMap[lastrow + i][lastcol - i] == gameMap[lastrow + i - 3][lastcol - i + 3]
            && gameMap[lastrow + i][lastcol - i] == gameMap[lastrow + i - 4][lastcol - i + 4]) {
                gameStatus = Status::Win;
                return true;
        }
    }
        // \方向
        for (int i = 0; i < 5; i++) {
            if (lastrow - i < chessBoardSize && lastrow - i - 4 >= 0 && lastcol - i >= 0
            && lastcol - i + 4 < chessBoardSize
            && gameMap[lastrow - i][lastcol - i] == gameMap[lastrow - i + 1][lastcol - i + 1]
            && gameMap[lastrow - i][lastcol - i] == gameMap[lastrow - i + 2][lastcol - i + 2]
            && gameMap[lastrow - i][lastcol - i] == gameMap[lastrow - i + 3][lastcol - i + 3]
            && gameMap[lastrow - i][lastcol - i] == gameMap[lastrow - i + 4][lastcol - i + 4]) {
                gameStatus = Status::Win;
                return true;
        }
    }
        return false;
}

bool Game::isDead() {
    if (coordinate.size() == chessBoardSize * chessBoardSize) {
        gameStatus = Status::Dead;
        return true;
    }
    return false;
}

void Game::calculateScore() {
    // 统计双方连成的子
    int personNum = 0;
    int aiNum     = 0;
    int emptyNum  = 0;
    // 评分数组清零
    clearMap(scoreMap);
    /*不同棋形不同评分
     * 眠二（可以形成眠三）        黑5       白5
     * 活二（可以形成活三）        黑10      白10
     * 眠三（可以形成冲四）        黑30      白25
     * 活三（可以形成活四）        黑40      白50
     * 冲四（有一个点可以连五）     黑60      白55
     * 活四（有两个点可以连五）     黑200     白10000
     * 连五（五个棋子连在一起）     黑20000   白30000
     *
     * 注意：分值调整
     * 例如如果场上有两个“活三”，应该和场上有一个“活四”差不多得分，因为都是已经赢了。
     */
    for (int row = 0; row < chessBoardSize; row++) {      // 行
        for (int col = 0; col < chessBoardSize; col++) {  // 列
            if (isEmptyAt(row, col)) {                    // 针对空白点算分
                // 遍历8个方向：【x水平方向】左-1，中0，右1；【y竖直方向】下-1，中0，上1
                for (int x = -1; x <= 1; x++) {
                    for (int y = -1; y <= 1; y++) {
                        personNum = 0;
                        aiNum     = 0;
                        emptyNum  = 0;  // 重置

                        if (!(x == 0 && y == 0)) {  // 中间的点不算
                            // 对黑子评分
                            // 每个方向延伸4个子
                            for (int i = 1; i <= 4; i++) {  // 正
                                int rowNow = row + i * y;
                                int colNow = col + i * x;
                                if (rowNow >= 0 && colNow >= 0 && rowNow < chessBoardSize
                                    && colNow < chessBoardSize && isBlackAt(rowNow, colNow)) {
                                    personNum++;
                                }
                                else if (rowNow >= 0 && colNow >= 0 && rowNow < chessBoardSize
                                         && colNow < chessBoardSize && isEmptyAt(rowNow, colNow)) {
                                    emptyNum++;
                                    break;
                                }
                                else
                                    break;
                            }

                            for (int i = 1; i <= 4; i++) {  // 反
                                int rowNow = row - i * y;
                                int colNow = col - i * x;
                                if (rowNow >= 0 && colNow >= 0 && rowNow < chessBoardSize
                                    && colNow < chessBoardSize && isBlackAt(rowNow, colNow)) {
                                    personNum++;
                                }
                                else if (rowNow >= 0 && colNow >= 0 && rowNow < chessBoardSize
                                         && colNow < chessBoardSize && isEmptyAt(rowNow, colNow)) {
                                    emptyNum++;
                                    break;
                                }
                                else
                                    break;
                            }

                            if (personNum == 1) {
                                if (emptyNum == 1)
                                    scoreMap[row][col] += 10;  // 眠二
                                else if (emptyNum == 2)
                                    scoreMap[row][col] += 5;  // 活二
                            }
                            else if (personNum == 2) {
                                if (emptyNum == 1)
                                    scoreMap[row][col] += 30;  // 眠三
                                else if (emptyNum == 2)
                                    scoreMap[row][col] += 40;  // 活三
                            }
                            else if (personNum == 3) {
                                if (emptyNum == 1)
                                    scoreMap[row][col] += 60;  // 眠四
                                else if (emptyNum == 2)
                                    scoreMap[row][col] += 200;  // 活四
                            }
                            else if (personNum == 4) {
                                scoreMap[row][col] += 20000;  // 连五
                            }

                            emptyNum = 0;  // 清空

                            // 对白棋评分
                            for (int i = 1; i <= 4; i++) {  // 正
                                int rowNow = row + i * y;
                                int colNow = col + i * x;
                                if (rowNow >= 0 && colNow >= 0 && rowNow < chessBoardSize
                                    && colNow < chessBoardSize && isWhiteAt(rowNow, colNow)) {
                                    aiNum++;
                                }
                                else if (rowNow >= 0 && colNow >= 0 && rowNow < chessBoardSize
                                         && colNow < chessBoardSize && isEmptyAt(rowNow, colNow)) {
                                    emptyNum++;
                                    break;
                                }
                                else
                                    break;
                            }

                            for (int i = 1; i <= 4; i++) {  // 反
                                int rowNow = row - i * y;
                                int colNow = col - i * x;
                                if (rowNow >= 0 && colNow >= 0 && rowNow < chessBoardSize
                                    && colNow < chessBoardSize && isWhiteAt(rowNow, colNow)) {
                                    aiNum++;
                                }
                                else if (rowNow >= 0 && colNow >= 0 && rowNow < chessBoardSize
                                         && colNow < chessBoardSize && isEmptyAt(rowNow, colNow)) {
                                    emptyNum++;
                                    break;
                                }
                                else
                                    break;
                            }

                            if (aiNum == 0)  // 普通下子
                                scoreMap[row][col] += 5;
                            else if (aiNum == 1) {
                                if (emptyNum == 1)
                                    scoreMap[row][col] += 10;  // 眠二
                                else if (emptyNum == 2)
                                    scoreMap[row][col] += 5;  // 活二
                            }
                            else if (aiNum == 2) {
                                if (emptyNum == 1)
                                    scoreMap[row][col] += 25;  // 眠三
                                else if (emptyNum == 2)
                                    scoreMap[row][col] += 50;  // 活三
                            }
                            else if (aiNum == 3) {
                                if (emptyNum == 1)
                                    scoreMap[row][col] += 55;  // 眠四
                                else if (emptyNum == 2)
                                    scoreMap[row][col] += 10000;  // 活四
                            }
                            else if (aiNum == 4) {
                                scoreMap[row][col] += 30000;  // 连五
                            }
                        }
                    }
                }
            }
        }
    }
}

Position Game::getBestChess() {
    // 计算积分
    calculateScore();
    int                   maxScore = 0;
    std::vector<Position> maxPoints;
    for (int row = 0; row < chessBoardSize; row++) {
        for (int col = 0; col < chessBoardSize; col++) {
            if (isEmptyAt(row, col)) {
                if (scoreMap[row][col] > maxScore) {
                    maxScore = scoreMap[row][col];
                    maxPoints.clear();
                    maxPoints.push_back(Position(row, col));
                }
                else if (scoreMap[row][col] == maxScore) {
                    maxPoints.push_back(Position(row, col));
                }
            }
        }
    }
    // 如果有多个分数相同的点，随机落点
    int index = rand() % maxPoints.size();
    return maxPoints[index];
}
