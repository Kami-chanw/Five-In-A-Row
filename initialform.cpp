#include "initialform.h"
#include "mainwindow.h"
#include "ui_initialform.h"
#include <QPushButton>

InitialForm::InitialForm(QWidget* parent) : ShadowWidget<QWidget>(parent), ui(new Ui::InitialForm) {
    ui->setupUi(this);
    initShadowWidget(ui->mainLayout);
    setBackgroundColor(Qt::white);
    title()->setIconColor(QColor(0x21, 0x21, 0x21));
    title()->setButtons(TitleBar::Close);
    setResizable(false);

    connect(ui->btnPVE, &QPushButton::clicked, this, [=] { showMainWindow(GameType::PVE); });
    connect(ui->btnPVP, &QPushButton::clicked, this, [=] { showMainWindow(GameType::PVP); });
}

InitialForm::~InitialForm() { delete ui; }

void InitialForm::showMainWindow(GameType type) {
    MainWindow* mainWnd = new MainWindow(type);
    mainWnd->setAttribute(Qt::WA_DeleteOnClose);
    connect(mainWnd, &MainWindow::showInitialForm, this, [=] {
        show();
    });
    mainWnd->show();
    hide();
}
