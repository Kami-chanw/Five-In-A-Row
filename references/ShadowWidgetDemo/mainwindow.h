#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ShadowWidget.h"
class MainWindow : public ShadowWidget<QMainWindow>
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
};
#endif // MAINWINDOW_H
