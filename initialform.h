#ifndef INITIALFORM_H
#define INITIALFORM_H

#include <QDialog>
#include "Game.h"
#include "ShadowWidget.h"

namespace Ui {
  class InitialForm;
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
  Ui::InitialForm *ui;
};

#endif // INITIALFORM_H
