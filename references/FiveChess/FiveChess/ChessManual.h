#pragma once

#include "ChessEngine.h"

#include <string>
#include <vector>

using namespace std;

class ChessManual
{
public:
    ChessManual();
    ChessManual(const vector<ChessEngine::Position>& v, int whichFirst);
    ~ChessManual();
    bool ReadAndCheck(TCHAR *filename);         //��ȡ����飨��ʽ���Ϸ��Եȣ�
    string Next();                              //��һ��
    string Previous();                          //��һ��
    void SaveToFile(TCHAR *filename);           //���浽�ļ�
    ChessEngine::Position GetLastPosition();    //��ȡ��ǰ����λ��

private:
    int whichFirst;                         //���ӻ��ǰ�������
    int currentIndex;                       //��ǰ��������
    vector<ChessEngine::Position> moves;    //�������в���
    string board;                           //���浱ǰ���̲���
    char currentRole;                       //��ǰ���ӽ�ɫ��'1'��'2'

    void changeRole() {
        currentRole = currentRole == '1' ? '2' : '1';
    }
};

