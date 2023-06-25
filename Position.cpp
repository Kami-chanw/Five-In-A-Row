#include "Position.h"

void Position::setRow(int r) { rowPos = r; }

void Position::setCol(int c) { colPos = c; }

int Position::row() const {
    return rowPos;  // 返回横坐标
}
int Position::col() const {
    return colPos;  // 返回棋子纵坐标
}
