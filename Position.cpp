#include "Position.h"

Position::Position(int row, int col, int sco) {
    setRowPos(row);
    setColPos(col);
    setScore(sco);
}

void Position::setRowPos(int r) { rowPos = r; }

void Position::setColPos(int c) { colPos = c; }
void Position::setScore(int s) { score = s; }

int Position::getRowPos() {
    return rowPos;  // 返回横坐标
}
int Position::getColPos() {
    return colPos;  // 返回棋子纵坐标
}
int Position::getScore() { return score; }
