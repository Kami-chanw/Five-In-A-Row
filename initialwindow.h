
#ifndef INITIALWINDOW_H
#define INITIALWINDOW_H

#include <QMainWindow>
#include <QDebug> //调试输出到控制台头文件
#include <QMessageBox> //消息提示框头文件

#include "mainwindow.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class InitialWindow : public QMainWindow
{
    Q_OBJECT
public:
    InitialWindow(QWidget *parent = nullptr);
    ~InitialWindow();
private:
    Ui::MainWindow *ui;
private slots:
    void jumpPage();//实现从开始菜单跳转到棋盘页面
};

#endif // INITIALWINDOW_H
