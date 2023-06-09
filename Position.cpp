#include "Position.h"

Position::Position(int row, int col)
{
    setRowPos(row);
    setColPos(col);
}

void Position::setRowPos(int r)
{
    rowPos = r;
}

void Position::setColPos(int c)
{
    colPos = c;
}

int Position::getRowPos()
{
    return rowPos;  // 返回横坐标
}
int Position::getColPos()
{
    return colPos;  // 返回棋子纵坐标
}
