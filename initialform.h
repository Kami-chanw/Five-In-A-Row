#ifndef INITIALFORM_H
#define INITIALFORM_H

#include "chessboard.h"
#include "shadowwidget.h"
#include <QDialog>

namespace Ui {
    class InitialForm;
}

class InitialForm : public ShadowWidget<QWidget> {
    Q_OBJECT

public:
    explicit InitialForm(QWidget* parent = nullptr);
    ~InitialForm();

private:
    Ui::InitialForm* ui;

    void showMainWindow(GameType type);
};

#endif  // INITIALFORM_H
