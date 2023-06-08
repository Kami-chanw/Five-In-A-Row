#ifndef INITIALFORM_H
#define INITIALFORM_H

#include <QDialog>
#include "Game.h"
#include "ShadowWidget.h"

namespace Ui {
  class InitialDialog;
}

class InitialForm : public ShadowWidget<QWidget>
{
  Q_OBJECT

public:
  explicit InitialForm(QWidget *parent = nullptr);
  ~InitialForm();

signals:
  void showMainWindow(GameType);

private:
  Ui::InitialDialog *ui;
};

#endif // INITIALFORM_H
