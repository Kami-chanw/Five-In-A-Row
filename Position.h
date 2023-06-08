
#ifndef POSITION_H
#define POSITION_H
class Position {
public:
    Position(int, int, int = 0);
    void setRowPos(int);
    void setColPos(int);
    void setScore(int);
    int  getRowPos();
    int  getColPos();
    int  getScore();

private:
    int rowPos;
    int colPos;
    int score;
};

#endif  // POSITION_H
