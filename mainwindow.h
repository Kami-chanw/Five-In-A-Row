#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Game.h"
#include "ShadowWidget.h"
#include <QMainWindow>

class Game;
class ChessBoard;
class Avatar;

class MainWindow : public ShadowWidget<QMainWindow> {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

signals:
    void showInitialForm();


private:
    ChessBoard* chessBoard = nullptr;
    Avatar* playerA,*playerB;

public:
    void initGame(GameType);
};


#endif  // MAINWINDOW_H
