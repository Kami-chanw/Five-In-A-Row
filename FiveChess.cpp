#include"FiveChess.h"
#include<vector>
using namespace std;
FiveChess::FiveChess(GameModel model,Difficulty difficulty)
{
    //获取游戏模式、难度
    this->model=model;
    this->difficulty=difficulty;
    //初始化棋盘
    /*for(int i=0;i<chessboardSize;++i)
    {
        for(int j=0;j<chessboardSize;++j)
        {
            Map[i][j]=0;
        }
    }*/

    /*Pos pos;
    for(pos.row=0;pos.row<chessboardSize;++pos.row)
    {
        for(pos.col=0;pos.col<chessboardSize;++pos.col)
        {
            pos.chess=0;
            Map.push_back(pos);
        }
    }*/
    //初始化骑手标识
    playerFlag=true;
}

void FiveChess::setChess(int row,int col)
{

    /*if(Map[row][col]==0)
    {
        Pos pos;
        pos.row=row;pos.col=col;
        if(playerFlag)
        {
            Map[row][col]=1;
            pos.chess=1;
        }
        else
        {
            Map[row][col]=-1;
            pos.chess=-1;
        }
        playerFlag=!playerFlag;
        Data.push_back(pos);
    }
    else{
        //warning或者无效点击
    }*/
    if(1/*再想：设计判断是否为空*/)
    {
        Pos pos;
        pos.row=row;pos.col=col;
        if(playerFlag)
        {
            pos.chess=1;
        }
        else
        {
            pos.chess=-1;
        }
        playerFlag=!playerFlag;
        Map.push_back(pos);
    }
    else{
        //warning或者无效点击
    }

}

Pos FiveChess::getLastPos()
{
    return Map.back();
    //return Data.rbegin();
}
bool FiveChess::isWin()
{
    //方案二：直接vector<position>全部遍历 待实现 算法效率不太行？
    int lastrow=getLastPos().row;
    int lastcol=getLastPos().col;
    //方案一：将vector<position>转为二维数组
    int map[chessboardSize][chessboardSize]={0};
    int r,c;
    for(int i=0;i<Map.size();i++)
    {
        r=Map[i].row;c=Map[i].col;
        map[r][c]=Map[i].chess;
    }
   //原理：从刚才落子的位置判断，判断其4个方向上是否有五颗连续的同类棋子
    //水平方向:左0右+4，左-1右+3，左-2右+2，左-3右+1，左-4右0
    for(int i=0;i<5;i++)
   {
       if(lastcol-i>=0&&lastcol-i+4<=chessboardSize&&
            map[lastrow][lastcol-i]==map[lastrow][lastcol-i+1]&&
            map[lastrow][lastcol-i]==map[lastrow][lastcol-i+2]&&
            map[lastrow][lastcol-i]==map[lastrow][lastcol-i+3]&&
            map[lastrow][lastcol-i]==map[lastrow][lastcol-i+4]&&)
            return true;
   }
    //竖直方向
   for(int i=0;i<5;i++)
   {
       if(lastrow-i>=0&&lastrow-i+4<=chessboardSize&&
           map[lastrow-i][lastcol]==map[lastrow-i+1][lastcol]&&
           map[lastrow-i][lastcol]==map[lastrow-i+2][lastcol]&&
           map[lastrow-i][lastcol]==map[lastrow-i+3][lastcol]&&
           map[lastrow-i][lastcol]==map[lastrow-i+4][lastcol]&&)
            return true;
   }
   //  /方向
   for(int i=0;i<5;i++)
   {
       if(lastrow+i<chessboardSize&&
           lastrow+i-4>=0&&
           lastcol-i>=0&&
           lastcol-i+4<chessboardSize&&
           map[lastrow+i][lastcol-i]==map[lastrow+i-1][lastcol-i+1]&&
           map[lastrow+i][lastcol-i]==map[lastrow+i-2][lastcol-i+2]&&
           map[lastrow+i][lastcol-i]==map[lastrow+i-3][lastcol-i+3]&&
           map[lastrow+i][lastcol-i]==map[lastrow+i-4][lastcol-i+4])
            return true;
   }
   // \方向
   for(int i=0;i<5;i++)
   {
       if(lastrow-i<chessboardSize&&
           lastrow-i-4>=0&&
           lastcol-i>=0&&
           lastcol-i+4<chessboardSize&&
           map[lastrow-i][lastcol-i]==map[lastrow-i+1][lastcol-i+1]&&
           map[lastrow-i][lastcol-i]==map[lastrow-i+2][lastcol-i+2]&&
           map[lastrow-i][lastcol-i]==map[lastrow-i+3][lastcol-i+3]&&
           map[lastrow-i][lastcol-i]==map[lastrow-i+4][lastcol-i+4])
            return true;
   }
   return false;

}

bool FiveChess::isDead()
{
   /*for(int i=0;i<chessboardSize;++i)
   {
       for(int j=0;j<chessboardSize;++j)
       {
            if(Map[i][j]==1||Map[i][j]==-1)
                return false;
       }
   }
   return true;*/

   if(Map.size()==chessboardSize*chessboardSize)
       return true;
   return false;
}
