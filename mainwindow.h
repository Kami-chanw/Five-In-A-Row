#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chessboard.h"
#include "shadowwidget.h"
#include <QMainWindow>

class AvatarWidget;

class MainWindow : public ShadowWidget<QMainWindow> {
    Q_OBJECT
    Q_PROPERTY(GameType gameType READ gameType WRITE setGameType)
public:
    MainWindow(GameType type, QWidget* parent = nullptr);
    explicit MainWindow(QWidget* parent = nullptr) : MainWindow(GameType::PVE, parent) {}
    ~MainWindow();

    GameType gameType() const { return chessBoard->gameType(); }
    void     setGameType(GameType type) { chessBoard->setGameType(type); }

signals:
    void showInitialForm();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    ChessBoard*   chessBoard = nullptr;
    AvatarWidget *playerA, *playerB;

    void initGame();
};

class MainWindowFactory : public QObject {
    Q_OBJECT

public:
    explicit MainWindowFactory(QObject* parent = nullptr);
    Q_INVOKABLE static void createAndShow(const QString& str);

    static MainWindowFactory* instance();
};

#endif  // MAINWINDOW_H
