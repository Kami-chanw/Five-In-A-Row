#include "initialform.h"
#include "ui_initialform.h"
#include <QPushButton>

InitialForm::InitialForm(QWidget* parent)
    : ShadowWidget<QWidget>(parent), ui(new Ui::InitialForm) {
    ui->setupUi(this);
    initShadowWidget(ui->mainLayout);
    setBackgroundColor(Qt::white);
    title()->setIconColor(QColor(0x21, 0x21, 0x21));
    title()->setButtons(TitleBar::Close);
    setResizable(false);

    connect(this, &InitialForm::showMainWindow, this, [&] { show(); });

    connect(ui->btnPVE, &QPushButton::clicked, this, [&] { emit showMainWindow(GameType::PVE); });
    connect(ui->btnPVP, &QPushButton::clicked, this, [&] { emit showMainWindow(GameType::PVP); });
}

InitialForm::~InitialForm() { delete ui; }
