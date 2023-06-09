#pragma once

#include <string>
#include <vector>
using namespace std;

namespace ChessEngine {

    enum Role { HUMAN = 1, COMPUTOR = 2, EMPTY = 0 };

    //λ�ýṹ�壬����x������y
    struct Position {
        int x;
        int y;
        int score;
        Position() {}
        Position(int x, int y) {
            this->x = x;
            this->y = y;
            score = 0;
        }
        Position(int x, int y, int score) {
            this->x = x;
            this->y = y;
            this->score = score;
        }
        bool operator <(const Position &pos) const {
            if (score != pos.score) {
                return score > pos.score;
            }
            if (x != pos.x) {
                return x < pos.x;
            }
            else {
                return y < pos.y;
            }
        }
    };


    //�ڿ�ʼ֮ǰ��һЩ��ʼ������
    void beforeStart();

    //�ж��Ƿ���ĳһ��Ӯ��
    int isSomeOneWin();

    //����
    string takeBack();

    //���֮ǰ�ļ�¼�����¿���
    string reset(int role);

    //�������ò���
    void setLevel(int level);

    //ȡ�øղŵ����µ���һ�����ӵ�λ��
    Position getLastPosition();

    //�������壬�������̣���������
    string nextStep(int x, int y);

    //��ȡ����
    vector<Position> getChessManual();


};
//namespace end
