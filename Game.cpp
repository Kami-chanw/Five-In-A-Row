#include "Game.h"
#include "ACSearcher.h"
#include "PossiblePositionManager.h"

#include <Set>
#include <cstring>
#include <iostream>
#include <stack>
#include <string>
#include <vector>
using namespace std;
struct Pattern {
    string pattern;
    int    score;
};
//模式
std::vector<Pattern> patterns = {
    { "11111", 50000 }, { "011110", 4320 }, { "011100", 720 }, { "001110", 720 },
    { "011010", 720 },  { "010110", 720 },  { "11110", 720 },  { "01111", 720 },
    { "11011", 720 },   { "10111", 720 },   { "11101", 720 },  { "001100", 120 },
    { "001010", 120 },  { "010100", 120 },  { "000100", 20 },  { "001000", 20 },
};

//生成64位随机数
long long random64() {
    return (long long)rand() | ((long long)rand() << 15) | ((long long)rand() << 30)
           | ((long long)rand() << 45) | ((long long)rand() << 60);
}
Game::Game() {}
Game::~Game() {}

void Game::startGame(GameType type) {
    //获取游戏模式
    gameType = type;
    // this->difficulty=difficulty;
    //初始化棋盘
    clearGameMap();
    //初始化评分数组
    for (int i = 0; i < chessboardSize; i++) {
        std::vector<int> temp;
        for (int i = 0; i < chessboardSize; i++) {
            temp.push_back(0);
        }
        scoreMap.push_back(temp);

        memset(board, EMPTY, chessboardSize * chessboardSize * sizeof(char));
        memset(scores, 0, sizeof(scores));
        vector<string> patternStrs;
        for (size_t i = 0; i < patterns.size(); i++) {
            patternStrs.push_back(patterns[i].pattern);
        }
        //初始化ACSearcher
        acs.LoadPattern(patternStrs);
        acs.BuildGotoTable();
        acs.BuildFailTable();
        randomBoardZobristValue();
        currentZobristValue = random64();
    }

    player = true;
}
void Game::clearGameMap() {
    for (int i = 0; i < chessboardSize; i++) {
        std::vector<int> temp;
        for (int i = 0; i < chessboardSize; i++) {
            temp.push_back(0);
        }
        gameMap.push_back(temp);
    }
}
void Game::updateGameMap(Position pos) {
    if (EmptyOrNot(pos))  //判断是否为空
    {
        if (player) {
            gameMap[pos.getRowPos()][pos.getColPos()] = -1;
        }
        else {
            gameMap[pos.getRowPos()][pos.getColPos()] = 1;
        }
        player = !player;
        coordinate.push_back(pos);
    }
}

void Game::act_P(Position pos) {
    coordinate.push(pos);
    int x       = pos.getRowPos();
    int y       = pos.getColPos();
    board[x][y] = HUMAN;
    currentZobristValue ^= boardZobristValue[HUMAN - 1][x][y];
    updateScore(board, pos);

    //增加可能出现的位置
    ppm.AddPossiblePositions(board, pos);

    Position result = getAGoodMove(board);

    board[result.getRowPos()][result.getColPos()] = COMPUTOR;
    currentZobristValue ^= boardZobristValue[COMPUTOR - 1][result.x][result.y];
    updateScore(board, result);

    //增加可能出现的位置
    ppm.AddPossiblePositions(board, result);

    //若双方还未决出胜负，则把棋子位置加入到历史记录中
    if (winner == -1)
        coordinate.push(Position(result.getRowPos(), result.getColPos()));

    string resultStr;
    int    i, j;
    for (i = 0; i < chessboardSize; i++) {
        for (j = 0; j < chessboardSize; j++) {
            resultStr.push_back(board[i][j] + 48);
        }
    }

    printBoard(board);
}
void Game::act_E(Position pos) { updateGameMap(getBestChess()); }

bool Game::EmptyOrNot(Position pos) {
    if (gameMap[pos.getRowPos()][pos.getColPos()] == 0) {
        return true;
    }
    return false;
}

Position Game::getLastPos() {
    return coordinate.back();
    // return Data.rbegin();
}
bool Game::WinOrNot() {
    /*//方案二：直接vector<position>全部遍历 待实现 算法效率不太行？
    int lastrow=getLastPos().row;
    int lastcol=getLastPos().col;
    //方案一：将vector<position>转为二维数组
    int map[chessboardSize][chessboardSize]={0};
    int r,c;
    for(int i=0;i<chessBoard.size();i++)
    {
        r=chessBoard[i].row;c=chessBoard[i].col;
        map[r][c]=chessBoard[i].chess;
    }
    int lastrow=getLastPos().getRowPos();
    int lastcol=getLastPos().getColPos();
   //原理：从刚才落子的位置判断，判断其4个方向上是否有五颗连续的同类棋子
    //水平方向:左0右+4，左-1右+3，左-2右+2，左-3右+1，左-4右0
    for(int i=0;i<5;i++)
   {
       if(lastcol-i>=0&&lastcol-i+4<=chessboardSize&&
            gameMap[lastrow][lastcol-i]==gameMap[lastrow][lastcol-i+1]&&
            gameMap[lastrow][lastcol-i]==gameMap[lastrow][lastcol-i+2]&&
            gameMap[lastrow][lastcol-i]==gameMap[lastrow][lastcol-i+3]&&
            gameMap[lastrow][lastcol-i]==gameMap[lastrow][lastcol-i+4]&&)
            return true;
   }
    //竖直方向
   for(int i=0;i<5;i++)
   {
       if(lastrow-i>=0&&lastrow-i+4<=chessboardSize&&
           gameMap[lastrow-i][lastcol]==gameMap[lastrow-i+1][lastcol]&&
           gameMap[lastrow-i][lastcol]==gameMap[lastrow-i+2][lastcol]&&
           gameMap[lastrow-i][lastcol]==gameMap[lastrow-i+3][lastcol]&&
           gameMap[lastrow-i][lastcol]==gameMap[lastrow-i+4][lastcol]&&)
            return true;
   }
   //  /方向
   for(int i=0;i<5;i++)
   {
       if(lastrow+i<chessboardSize&&
           lastrow+i-4>=0&&
           lastcol-i>=0&&
           lastcol-i+4<chessboardSize&&
           gameMap[lastrow+i][lastcol-i]==gameMap[lastrow+i-1][lastcol-i+1]&&
           gameMap[lastrow+i][lastcol-i]==gameMap[lastrow+i-2][lastcol-i+2]&&
           gameMap[lastrow+i][lastcol-i]==gameMap[lastrow+i-3][lastcol-i+3]&&
           gameMap[lastrow+i][lastcol-i]==gameMap[lastrow+i-4][lastcol-i+4])
            return true;
   }
   // \方向
   for(int i=0;i<5;i++)
   {
       if(lastrow-i<chessboardSize&&
           lastrow-i-4>=0&&
           lastcol-i>=0&&
           lastcol-i+4<chessboardSize&&
           gameMap[lastrow-i][lastcol-i]==gameMap[lastrow-i+1][lastcol-i+1]&&
           gameMap[lastrow-i][lastcol-i]==gameMap[lastrow-i+2][lastcol-i+2]&&
           gameMap[lastrow-i][lastcol-i]==gameMap[lastrow-i+3][lastcol-i+3]&&
           gameMap[lastrow-i][lastcol-i]==gameMap[lastrow-i+4][lastcol-i+4])
            return true;
   }
   return false;*/
    if (winner == HUMAN)
        return true;
    if (winner == COMPUTOR)
        return false;
}

bool Game::DeadOrNot() {

    if (coordinate.size() == chessboardSize * chessboardSize)
        return true;
    return false;
}

//记录计算结果在哈希表中
void Game::recordHashItem(int depth, int score, HashItem::Flag flag) {
    int       index     = (int)(currentZobristValue & HASH_ITEM_INDEX_MASK);
    HashItem* phashItem = &hashItems[index];

    if (phashItem->flag != HashItem::EMPTY && phashItem->depth > depth) {
        return;
    }

    phashItem->checksum = currentZobristValue;
    phashItem->score    = score;
    phashItem->flag     = flag;
    phashItem->depth    = depth;
}

//在哈希表中取得计算好的局面的分数
int Game::getHashItemScore(int depth, int alpha, int beta) {
    int       index     = (int)(currentZobristValue & HASH_ITEM_INDEX_MASK);
    HashItem* phashItem = &hashItems[index];

    if (phashItem->flag == HashItem::EMPTY)
        return UNKNOWN_SCORE;

    if (phashItem->checksum == currentZobristValue) {
        if (phashItem->depth >= depth) {
            if (phashItem->flag == HashItem::EXACT) {
                return phashItem->score;
            }
            if (phashItem->flag == HashItem::ALPHA && phashItem->score <= alpha) {
                return alpha;
            }
            if (phashItem->flag == HashItem::BETA && phashItem->score >= beta) {
                return beta;
            }
        }
    }

    return UNKNOWN_SCORE;
}

//生成zobrist键值
void Game::randomBoardZobristValue() {
    int i, j, k;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < chessboardSize; j++) {
            for (k = 0; k < chessboardSize; k++) {
                boardZobristValue[i][j][k] = random64();
            }
        }
    }
}

//初始化初始局面的zobrist值
void Game::initCurrentZobristValue() { currentZobristValue = random64(); }

//根据位置评分，其中board是当前棋盘，p是位置，role是评分角色，比如role是Human则是相对人类评分，比如role是computer则是对于电脑评分
int Game::evaluatePoint(char board[chessboardSize][chessboardSize], Position p) {
    int          result;
    unsigned int i, j;
    int          role;

    result = 0;
    role   = HUMAN;

    string lines[4];
    string lines1[4];
    for (i = max(0, p.getRowPos() - 5); i < (unsigned int)min(chessboardSize, p.getRowPos() + 6);
         i++) {
        if (i != p.getRowPos()) {
            lines[0].push_back(board[i][p.getColPos()] == role ? '1'
                               : board[i][p.getColPos()] == 0  ? '0'
                                                               : '2');
            lines1[0].push_back(board[i][p.getColPos()] == role ? '2'
                                : board[i][p.getColPos()] == 0  ? '0'
                                                                : '1');
        }
        else {
            lines[0].push_back('1');
            lines1[0].push_back('1');
        }
    }
    for (i = max(0, p.getColPos() - 5); i < (unsigned int)min(chessboardSize, p.getColPos() + 6);
         i++) {
        if (i != p.getColPos()) {
            lines[1].push_back(board[p.getRowPos()][i] == role ? '1'
                               : board[p.getRowPos()][i] == 0  ? '0'
                                                               : '2');
            lines1[1].push_back(board[p.getRowPos()][i] == role ? '2'
                                : board[p.getRowPos()][i] == 0  ? '0'
                                                                : '1');
        }
        else {
            lines[1].push_back('1');
            lines1[1].push_back('1');
        }
    }
    for (i = p.getRowPos() - min(min(p.getRowPos(), p.getColPos()), 5),
        j  = p.getColPos() - min(min(p.getRowPos(), p.getColPos()), 5);
         i < (unsigned int)min(chessboardSize, p.getRowPos() + 6)
         && j < (unsigned int)min(chessboardSize, p.getColPos() + 6);
         i++, j++) {
        if (i != p.getRowPos()) {
            lines[2].push_back(board[i][j] == role ? '1' : board[i][j] == 0 ? '0' : '2');
            lines1[2].push_back(board[i][j] == role ? '2' : board[i][j] == 0 ? '0' : '1');
        }
        else {
            lines[2].push_back('1');
            lines1[2].push_back('1');
        }
    }
    for (i = p.getRowPos() + min(min(p.getColPos(), chessboardSize - 1 - p.getRowPos()), 5),
        j  = p.y - min(min(p.getColPos(), chessboardSize - 1 - p.getRowPos()), 5);
         i >= (unsigned int)max(0, p.getRowPos() - 5)
         && j < (unsigned int)min(chessboardSize, p.getColPos() + 6);
         i--, j++) {
        if (i != p.getRowPos()) {
            lines[3].push_back(board[i][j] == role ? '1' : board[i][j] == 0 ? '0' : '2');
            lines1[3].push_back(board[i][j] == role ? '2' : board[i][j] == 0 ? '0' : '1');
        }
        else {
            lines[3].push_back('1');
            lines1[3].push_back('1');
        }
    }

    for (i = 0; i < 4; i++) {
        vector<int> tmp = acs.ACSearch(lines[i]);
        for (j = 0; j < tmp.size(); j++) {
            result += patterns[tmp[j]].score;
        }

        tmp = acs.ACSearch(lines1[i]);
        for (j = 0; j < tmp.size(); j++) {
            result += patterns[tmp[j]].score;
        }
    }

    return result;
}

//局面评估函数，给一个局面评分
int Game::evaluate(char board[chessboardSize][chessboardSize], Role role) {

    if (role == COMPUTOR) {
        return allScore[1];
    }
    else if (role == HUMAN) {
        return allScore[0];
    }
    cout << "error" << endl;

    return 0;
}

void Game::updateScore(char board[chessboardSize][chessboardSize], Position p) {

    string       lines[4];
    string       lines1[4];
    unsigned int i, j;
    int          role = HUMAN;  // BLACK

    //竖
    for (i = 0; i < chessboardSize; i++) {

        lines[0].push_back(board[i][p.getColPos()] == role ? '1'
                           : board[i][p.getColPos()] == 0  ? '0'
                                                           : '2');
        lines1[0].push_back(board[i][p.getColPos()] == role ? '2'
                            : board[i][p.getColPos()] == 0  ? '0'
                                                            : '1');
    }
    //横
    for (i = 0; i < chessboardSize; i++) {

        lines[1].push_back(board[p.getRowPos()][i] == role ? '1'
                           : board[p.getRowPos()][i] == 0  ? '0'
                                                           : '2');
        lines1[1].push_back(board[p.getRowPos()][i] == role ? '2'
                            : board[p.getRowPos()][i] == 0  ? '0'
                                                            : '1');
    }
    //反斜杠
    for (i = p.getRowPos() - min(p.getRowPos(), p.getColPos()),
        j  = p.getColPos() - min(p.getRowPos(), p.getColPos());
         i < chessboardSize && j < chessboardSize; i++, j++) {

        lines[2].push_back(board[i][j] == role ? '1' : board[i][j] == 0 ? '0' : '2');
        lines1[2].push_back(board[i][j] == role ? '2' : board[i][j] == 0 ? '0' : '1');
    }
    //斜杠
    for (i = p.getRowPos() + min(p.getColPos(), chessboardSize - 1 - p.getRowPos()),
        j  = p.getColPos() - min(p.getColPos(), chessboardSize - 1 - p.getRowPos());
         i >= 0 && j < chessboardSize; i--, j++) {

        lines[3].push_back(board[i][j] == role ? '1' : board[i][j] == 0 ? '0' : '2');
        lines1[3].push_back(board[i][j] == role ? '2' : board[i][j] == 0 ? '0' : '1');
    }

    int lineScore[4];
    int line1Score[4];
    memset(lineScore, 0, sizeof(lineScore));
    memset(line1Score, 0, sizeof(line1Score));

    //计算分数
    for (i = 0; i < 4; i++) {
        vector<int> result = acs.ACSearch(lines[i]);
        for (j = 0; j < result.size(); j++) {
            lineScore[i] += patterns[result[j]].score;
        }

        result = acs.ACSearch(lines1[i]);
        for (j = 0; j < result.size(); j++) {
            line1Score[i] += patterns[result[j]].score;
        }
    }

    int a = p.getColPos();
    int b = chessboardSize + p.getRowPos();
    int c = 2 * chessboardSize + (p.getColPos() - p.getRowPos() + 10);
    int d = 2 * chessboardSize + 21 + (p.getRowPos() + p.getColPos() - 4);
    //减去以前的记录
    for (i = 0; i < 2; i++) {
        allScore[i] -= scores[i][a];
        allScore[i] -= scores[i][b];
    }

    // scores顺序 竖、横、\、/
    scores[0][a] = lineScore[0];
    scores[1][a] = line1Score[0];
    scores[0][b] = lineScore[1];
    scores[1][b] = line1Score[1];

    //加上新的记录
    for (i = 0; i < 2; i++) {
        allScore[i] += scores[i][a];
        allScore[i] += scores[i][b];
    }

    if (p.getColPos() - p.getRowPos() >= -10 && p.getColPos() - p.getRowPos() <= 10) {

        for (i = 0; i < 2; i++)
            allScore[i] -= scores[i][c];

        scores[0][c] = lineScore[2];
        scores[1][c] = line1Score[2];

        for (i = 0; i < 2; i++)
            allScore[i] += scores[i][c];
    }

    if (p.getRowPos() + p.getColPos() >= 4 && p.getRowPos() + p.getColPos() <= 24) {

        for (i = 0; i < 2; i++)
            allScore[i] -= scores[i][d];

        scores[0][d] = lineScore[3];
        scores[1][d] = line1Score[3];

        for (i = 0; i < 2; i++)
            allScore[i] += scores[i][d];
    }
}

// alpha-beta剪枝
int Game::abSearch(char board[chessboardSize][chessboardSize], int depth, int alpha, int beta,
                   Role currentSearchRole) {
    HashItem::Flag flag  = HashItem::ALPHA;
    int            score = getHashItemScore(depth, alpha, beta);
    if (score != UNKNOWN_SCORE && depth != DEPTH) {
        return score;
    }

    int score1 = evaluate(board, currentSearchRole);
    int score2 = evaluate(board, currentSearchRole == HUMAN ? COMPUTOR : HUMAN);

    if (score1 >= 50000) {
        return MAX_SCORE - 1000 - (DEPTH - depth);
    }
    if (score2 >= 50000) {
        return MIN_SCORE + 1000 + (DEPTH - depth);
    }

    if (depth == 0) {
        recordHashItem(depth, score1 - score2, HashItem::EXACT);
        return score1 - score2;
    }

    // set<Position> possiblePossitions = createPossiblePosition(board);

    int                  count = 0;
    set<Position>        possiblePositions;
    const set<Position>& tmpPossiblePositions = ppm.GetCurrentPossiblePositions();

    //对当前可能出现的位置进行粗略评分
    set<Position>::iterator iter;
    for (iter = tmpPossiblePositions.begin(); iter != tmpPossiblePositions.end(); iter++) {
        possiblePositions.insert(Position(iter->x, iter->y, evaluatePoint(board, *iter)));
    }

    while (!possiblePositions.empty()) {
        Position p = *possiblePositions.begin();

        possiblePositions.erase(possiblePositions.begin());

        //放置棋子
        board[p.getRowPos()][p.getColPos()] = currentSearchRole;
        currentZobristValue ^=
            boardZobristValue[currentSearchRole - 1][p.getRowPos()][p.getColPos()];
        updateScore(board, p);

        //增加可能出现的位置
        p.setScore(0);
        ppm.AddPossiblePositions(board, p);

        int val = -abSearch(board, depth - 1, -beta, -alpha,
                            currentSearchRole == HUMAN ? COMPUTOR : HUMAN);
        if (depth == DEPTH)
            cout << "score(" << p.getRowPos() << "," << p.getColPos() << "):" << val << endl;

        //取消上一次增加的可能出现的位置
        ppm.Rollback();

        //取消放置
        board[p.getRowPos()][p.getColPos()] = 0;
        currentZobristValue ^= boardZobristValue[currentSearchRole - 1][p.x][p.y];
        updateScore(board, p);

        if (val >= beta) {
            recordHashItem(depth, beta, HashItem::BETA);
            return beta;
        }
        if (val > alpha) {
            flag  = HashItem::EXACT;
            alpha = val;
            if (depth == DEPTH) {
                searchResult = p;
            }
        }

        count++;
        if (count >= 9) {
            break;
        }
    }

    recordHashItem(depth, alpha, flag);
    return alpha;
}

/*void Game::calculateScore()
{
   //统计双方连成的子
   int personNum=0;
   int aiNum=0;
   int emptyNum=0;
   //评分数组清零
   for(int i=0;i<chessboardSize;i++)
   {
       for(int j=0;j<chessboardSize;j++)
       {
            scoreMap[i][j]=0;
       }
   }
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
    *
   for(int row=0;row<chessboardSize;row++)//行
   {
       for(int col=0;col<chessboardSize;col++)//列
       {
            if(gameMap[row][col]==0)//针对空白点算分
            {
                //遍历8个方向：【x水平方向】左-1，中0，右1；【y竖直方向】下-1，中0，上1
                for(int x=-1;x<=1;x++)
                {
                    for(int y=-1;y<=1;y++)
                    {
                        personNum=0;
                        aiNum=0;
                        emptyNum=0;//重置

                        if(!(x==0&&y==0))//中间的点不算
                        {
                            //对黑子评分
                            //每个方向延伸4个子
                            for(int i=1;i<=4;i++)//正
                            {
                                int rowNow=row+i*y;
                                int colNow=col+i*x;
                                if(rowNow>=0&&colNow>=0&&
                                    rowNow<chessboardSize&&
                                    colNow<chessboardSize&&
                                    gameMap[rowNow][colNow]==-1)
                                {
                                    personNum++;}
                                else if(rowNow>=0&&colNow>=0&&
                                         rowNow<chessboardSize&&
                                         colNow<chessboardSize&&
                                         gameMap[rowNow][colNow]==0)
                                {
                                    emptyNum++;break;}
                                else
                                    break;
                            }

                            for(int i=1;i<=4;i++)//反
                            {
                                int rowNow=row-i*y;
                                int colNow=col-i*x;
                                if(rowNow>=0&&colNow>=0&&
                                    rowNow<chessboardSize&&
                                    colNow<chessboardSize&&
                                    gameMap[rowNow][colNow]==-1)
                                {
                                    personNum++;}
                                else if(rowNow>=0&&colNow>=0&&
                                         rowNow<chessboardSize&&
                                         colNow<chessboardSize&&
                                         gameMap[rowNow][colNow]==0)
                                {
                                    emptyNum++;break;}
                                else
                                    break;
                            }

                            if(personNum==1)
                            {
                                if(emptyNum==1)
                                    scoreMap[row][col]+=10;//眠二
                                else if(emptyNum==2)
                                    scoreMap[row][col]+=5;//活二
                            }
                            else if(personNum==2)
                            {
                                if(emptyNum==1)
                                    scoreMap[row][col]+=30;//眠三
                                else if(emptyNum==2)
                                    scoreMap[row][col]+=40;//活三
                            }
                            else if(personNum==3)
                            {
                                if(emptyNum==1)
                                    scoreMap[row][col]+=60;//眠四
                                else if(emptyNum==2)
                                    scoreMap[row][col]+=200;//活四
                            }
                            else if(personNum==4)
                            {
                                scoreMap[row][col]+=20000;//连五
                            }

                            emptyNum=0;//清空

                            //对白棋评分
                            for(int i=1;i<=4;i++)//正
                            {
                                int rowNow=row+i*y;
                                int colNow=col+i*x;
                                if(rowNow>=0&&colNow>=0&&
                                    rowNow<chessboardSize&&
                                    colNow<chessboardSize&&
                                    gameMap[rowNow][colNow]==1)
                                {
                                    aiNum++;}
                                else if(rowNow>=0&&colNow>=0&&
                                         rowNow<chessboardSize&&
                                         colNow<chessboardSize&&
                                         gameMap[rowNow][colNow]==0)
                                {
                                    emptyNum++;break;}
                                else
                                    break;
                            }

                            for(int i=1;i<=4;i++)//反
                            {
                                int rowNow=row-i*y;
                                int colNow=col-i*x;
                                if(rowNow>=0&&colNow>=0&&
                                    rowNow<chessboardSize&&
                                    colNow<chessboardSize&&
                                    gameMap[rowNow][colNow]==1)
                                {
                                    aiNum++;}
                                else if(rowNow>=0&&colNow>=0&&
                                         rowNow<chessboardSize&&
                                         colNow<chessboardSize&&
                                         gameMap[rowNow][colNow]==0)
                                {
                                    emptyNum++;break;}
                                else
                                    break;
                            }

                            if(aiNum==0)//普通下子
                                scoreMap[row][col]+=5;
                            else if(aiNum==1)
                            {
                                if(emptyNum==1)
                                    scoreMap[row][col]+=10;//眠二
                                else if(emptyNum==2)
                                    scoreMap[row][col]+=5;//活二
                            }
                            else if(aiNum==2)
                            {
                                if(emptyNum==1)
                                    scoreMap[row][col]+=25;//眠三
                                else if(emptyNum==2)
                                    scoreMap[row][col]+=50;//活三
                            }
                            else if(aiNum==3)
                            {
                                if(emptyNum==1)
                                    scoreMap[row][col]+=55;//眠四
                                else if(emptyNum==2)
                                    scoreMap[row][col]+=10000;//活四
                            }
                            else if(aiNum==4)
                            {
                                scoreMap[row][col]+=30000;//连五
                            }


                        }
                    }
                }
            }
       }
   }
}

Position Game::getBestChess()
{
   //计算积分
   calculateScore();
   int maxScore=0;
   std::vector<Position>maxPoints;
   int k=0;
   for(int row=0;row<chessboardSize;row++)
   {
       for(int col=0;col<chessboardSize;col++)
       {
            if(gameMap[row][col]==0)
            {
                if(scoreMap[row][col]>maxScore)
                {
                    maxScore=scoreMap[row][col];
                    maxPoints.clear();
                    maxPoints.push_back(Position(row,col));
                }
                else if(scoreMap[row][col]==maxScore)
                {
                    maxPoints.push_back(Position(row,col));
                }
            }

       }
       //如果有多个分数相同的点，随机落点
       int index=rand()%maxPoints.size();
       return maxPoints[index];
   }
}
*/
