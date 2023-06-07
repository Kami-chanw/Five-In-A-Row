#include"FiveChess.h"
using namespace std;
FiveChess::FiveChess(GameModel model,Difficulty difficulty)
{
    //获取游戏模式、难度
    this->model=model;
    this->difficulty=difficulty;
    //初始化棋盘
    for(int i=0;i<chessboardSize;++i)
    {
        for(int j=0;j<chessboardSize;++j)
        {
            Map[i][j]=0;
        }
    }
    //初始化骑手标识
    playerFlag=true;
}

void FiveChess::setChess(int row,int col)
{
    if(Map[row][col]==0)
    {
        if(playerFlag)
        {
            Map[row][col]=1;}
        else
        {
            Map[row][col]=-1;}
        playerFlag=!playerFlag;
    }
    else{
        //warning或者无效点击
    }

}
bool FiveChess::isWin(int lastrow,int lastcol)
{
   //原理：从刚才落子的位置判断，判断其4个方向上是否有五颗连续的同类棋子
    //水平方向:左0右+4，左-1右+3，左-2右+2，左-3右+1，左-4右0
    for(int i=0;i<5;i++)
   {
       if(lastcol-i>=0&&lastcol-i+4<=chessboardSize&&
            Map[lastrow][lastcol-i]==Map[lastrow][lastcol-i+1]&&
            Map[lastrow][lastcol-i]==Map[lastrow][lastcol-i+2]&&
            Map[lastrow][lastcol-i]==Map[lastrow][lastcol-i+3]&&
            Map[lastrow][lastcol-i]==Map[lastrow][lastcol-i+4]&&)
            return true;
   }
    //竖直方向
   for(int i=0;i<5;i++)
   {
       if(lastrow-i>=0&&lastrow-i+4<=chessboardSize&&
           Map[lastrow-i][lastcol]==Map[lastrow-i+1][lastcol]&&
           Map[lastrow-i][lastcol]==Map[lastrow-i+2][lastcol]&&
           Map[lastrow-i][lastcol]==Map[lastrow-i+3][lastcol]&&
           Map[lastrow-i][lastcol]==Map[lastrow-i+4][lastcol]&&)
            return true;
   }
   //  /方向
   for(int i=0;i<5;i++)
   {
       if(lastrow+i<chessboardSize&&
           lastrow+i-4>=0&&
           lastcol-i>=0&&
           lastcol-i+4<chessboardSize&&
           Map[lastrow+i][lastcol-i]==Map[lastrow+i-1][lastcol-i+1]&&
           Map[lastrow+i][lastcol-i]==Map[lastrow+i-2][lastcol-i+2]&&
           Map[lastrow+i][lastcol-i]==Map[lastrow+i-3][lastcol-i+3]&&
           Map[lastrow+i][lastcol-i]==Map[lastrow+i-4][lastcol-i+4])
            return true;
   }
   // \方向
   for(int i=0;i<5;i++)
   {
       if(lastrow-i<chessboardSize&&
           lastrow-i-4>=0&&
           lastcol-i>=0&&
           lastcol-i+4<chessboardSize&&
           Map[lastrow-i][lastcol-i]==Map[lastrow-i+1][lastcol-i+1]&&
           Map[lastrow-i][lastcol-i]==Map[lastrow-i+2][lastcol-i+2]&&
           Map[lastrow-i][lastcol-i]==Map[lastrow-i+3][lastcol-i+3]&&
           Map[lastrow-i][lastcol-i]==Map[lastrow-i+4][lastcol-i+4])
            return true;
   }
   return false;
}

bool FiveChess::isDead()
{
   for(int i=0;i<chessboardSize;++i)
   {
       for(int j=0;j<chessboardSize;++j)
       {
            if(Map[i][j]==1||Map[i][j]==-1)
                return false;
       }
   }
   return true;
}
