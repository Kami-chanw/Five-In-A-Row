#include "stdafx.h"
#include "ChessManual.h"

#include <fstream>

ChessManual::ChessManual()
    :whichFirst(0),
    currentIndex(-1)
{
    int i = 0;
    for (i = 0; i < 15 * 15; i++) {
        board.push_back('0');
    }
}

ChessManual::ChessManual(const vector<ChessEngine::Position>& v, int whichFirst)
    :whichFirst(whichFirst),
    currentIndex(-1),
    moves(v)
{
    int i = 0;
    for (i = 0; i < 15 * 15; i++) {
        board.push_back('0');
    }
}


bool ChessManual::ReadAndCheck(TCHAR *filename) {
    fstream fs(filename);

    //������ֻ��Ǻ�������
    fs >> whichFirst;
    if (whichFirst)
        currentRole = '2';
    else
        currentRole = '1';

    //��ʼ������
    int i, j;
    for (i = 0; i < 15; i++) {
        for (j = 0; j < 15; j++) {
            board[i * 15 + j] = '0';
        }
    }

    int x = -1, y = -1;
    while (fs >> x && fs >> y) {
        //�ж�x��y�Ƿ�Ϸ�
        if (x >= 0 && x <= 14 && y >= 0 && y <= 14) {
            moves.push_back(ChessEngine::Position(x, y));
            if (board[x * 15 + y] == '0')
                board[x * 15 + y] = '1';
            else
                return false;
        }

        x = -1, y = -1;
    }

    //�������Ϊ0������false
    if (!moves.size())
        return false;

    //��������
    for (i = 0; i < 15; i++) {
        for (j = 0; j < 15; j++) {
            board[i * 15 + j] = '0';
        }
    }

    fs.close();
    return true;
}

string ChessManual::Next() {
    if (currentIndex == moves.size() - 1)
        return "";

    currentIndex++;
    ChessEngine::Position pos = moves[currentIndex];

    //������������
    board[pos.x * 15 + pos.y] = currentRole;

    //ת�����ӽ�ɫ
    changeRole();

    return board;
}

string ChessManual::Previous() {
    if (currentIndex == -1)
        return "";

    ChessEngine::Position pos = moves[currentIndex];
    currentIndex--;

    //��������ȥ����λ�õ�����
    board[pos.x * 15 + pos.y] = '0';

    //ת�����ӽ�ɫ
    changeRole();

    return board;
}

void ChessManual::SaveToFile(TCHAR *filename) {
    fstream fs(filename, fstream::out);

    //д˭����
    fs << whichFirst;
    fs << '\n';

    //д����
    vector<ChessEngine::Position>::iterator iter;
    for (iter = moves.begin(); iter != moves.end(); iter++) {
        fs << (*iter).x << ' ' << (*iter).y << '\n';
    }

    fs.close();
}

ChessEngine::Position ChessManual::GetLastPosition() {
    if(currentIndex != -1)
        return moves[currentIndex];
    return ChessEngine::Position(-1, -1);
}

ChessManual::~ChessManual()
{
}
