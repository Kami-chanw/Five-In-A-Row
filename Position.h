
#ifndef POSITION_H
#define POSITION_H
class Position
{
public:
    Position(int,int);
    void setRowPos(int);
    void setColPos(int);
    int getRowPos();
    int getColPos();
private:
    int rowPos;
    int colPos;
};

#endif // POSITION_H
