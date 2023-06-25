#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Game.h"
#include "ShadowWidget.h"
#include <QMainWindow>

class Game;
class ChessBoard;
class AvatarWidget;

class MainWindow : public ShadowWidget<QMainWindow> {
    Q_OBJECT

public:
    MainWindow(GameType type, QWidget* parent = nullptr);
    ~MainWindow();

signals:
    void showInitialForm();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    ChessBoard* chessBoard = nullptr;
    AvatarWidget* playerA,*playerB;

    void initGame();
};


#endif  // MAINWINDOW_H
