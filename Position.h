
#ifndef POSITION_H
#define POSITION_H
class Position {
public:
    Position() : Position(-1, -1) {}
    Position(int row, int col) : rowPos(row), colPos(col) {}
    Position(const Position&) = default;
    Position(Position&&)      = default;

    Position& operator=(const Position&) = default;
    Position& operator=(Position&&)      = default;

    bool operator==(const Position& pos) const {
        return colPos == pos.colPos && rowPos == pos.rowPos;
    }
    bool operator!=(const Position& pos) const { return !(*this == pos); }

    void setRow(int);
    void setCol(int);
    int  row() const;
    int  col() const;

    bool isValid() const { return rowPos >= 0 && colPos >= 0; }
    bool isValid(int maxRow, int maxCol = -1) const {
        return isValid() && rowPos < maxRow && colPos < (maxCol >= 0 ? maxCol : maxRow);
    }

private:
    int rowPos;
    int colPos;
};

#endif  // POSITION_H
