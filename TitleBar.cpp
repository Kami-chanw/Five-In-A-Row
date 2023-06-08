#include "TitleBar.h"
#include <QBitmap>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QGraphicsColorizeEffect>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>
#undef GLOBAL_POS
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
#define GLOBAL_POS(e) e->globalPosition().toPoint()
#else
#define GLOBAL_POS(e) e->globalPos()
#endif

void TitleBar::TitleBarUi::setupUi(QWidget* titleBar) {
    titleBar->resize(267, 36);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy.setHeightForWidth(titleBar->sizePolicy().hasHeightForWidth());
    titleBar->setSizePolicy(sizePolicy);
    titleBar->setStyleSheet(QString::fromUtf8("background:transparent;"));
    layout = new QGridLayout(titleBar);
    layout->setSpacing(2);
    layout->setContentsMargins(11, 2, 2, 2);
    lblTitle = new QLabel(titleBar);
    sizePolicy.setHeightForWidth(titleBar->sizePolicy().hasHeightForWidth());
    lblTitle->setSizePolicy(sizePolicy);
    lblTitle->setStyleSheet(QString::fromUtf8("QLabel {\n"
                                              "	background: transparent;\n"
                                              "}"));

    layout->addWidget(lblTitle, 0, 0, 1, 1);

    horizontalSpacer = new QSpacerItem(169, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    layout->addItem(horizontalSpacer, 0, 1, 1, 1);

    btnMinimize = new QPushButton(titleBar);
    titleBar->connect(btnMinimize, &QPushButton::clicked, titleBar,
                      [=] { titleBar->showMinimized(); });
    QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy1.setHeightForWidth(btnMinimize->sizePolicy().hasHeightForWidth());
    btnMinimize->setSizePolicy(sizePolicy1);
    btnMinimize->setMinimumSize(QSize(32, 32));
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/xicons/minimize.svg"), QSize(), QIcon::Normal, QIcon::Off);
    btnMinimize->setIcon(icon);
    btnMinimize->setIconSize(QSize(21, 21));

    layout->addWidget(btnMinimize, 0, 2, 1, 1);

    btnMaximize = new QPushButton(titleBar);
    titleBar->connect(btnMaximize, &QPushButton::clicked, titleBar,
                      [=] { titleBar->showMaximized(); });
    sizePolicy1.setHeightForWidth(btnMaximize->sizePolicy().hasHeightForWidth());
    btnMaximize->setSizePolicy(sizePolicy1);
    btnMaximize->setMinimumSize(QSize(32, 32));
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/xicons/maximize.svg"), QSize(), QIcon::Normal, QIcon::Off);
    btnMaximize->setIcon(icon1);
    btnMaximize->setIconSize(QSize(21, 21));

    layout->addWidget(btnMaximize, 0, 3, 1, 1);

    btnRestore = new QPushButton(titleBar);
    titleBar->connect(btnRestore, &QPushButton::clicked, titleBar, [=] { titleBar->showNormal(); });
    sizePolicy1.setHeightForWidth(btnRestore->sizePolicy().hasHeightForWidth());
    btnRestore->setSizePolicy(sizePolicy1);
    btnRestore->setMinimumSize(QSize(32, 32));
    QIcon icon2;
    icon2.addFile(QString::fromUtf8(":/xicons/restore.svg"), QSize(), QIcon::Normal, QIcon::Off);
    btnRestore->setIcon(icon2);
    btnRestore->setIconSize(QSize(21, 21));

    layout->addWidget(btnRestore, 0, 4, 1, 1);

    btnClose = new QPushButton(titleBar);
    titleBar->connect(btnClose, &QPushButton::clicked, titleBar, [=] { titleBar->close(); });
    QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sizePolicy2.setHeightForWidth(btnClose->sizePolicy().hasHeightForWidth());
    btnClose->setSizePolicy(sizePolicy2);
    btnClose->setMinimumSize(QSize(32, 32));
    QIcon icon3;
    icon3.addFile(QString::fromUtf8(":/xicons/close.svg"), QSize(), QIcon::Normal, QIcon::Off);
    btnClose->setIcon(icon3);
    btnClose->setIconSize(QSize(21, 21));

    layout->addWidget(btnClose, 0, 5, 1, 1);
}

TitleBar::TitleBar(QWidget* parentWidget, TitleButtons button, QWidget* parent)
    : QWidget(parent), flag(button), parentWidget(parentWidget) {
    ui.setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint, true);
    installEventFilter(this);
    setButtons(button);
    if (parentWidget && parentWidget->layout()) {
        auto contentMargins = parentWidget->layout()->contentsMargins();
        parentWidget->layout()->setContentsMargins(QMargins(0, 0, 0, contentMargins.bottom()));
    }
    connect(ui.btnClose, &QPushButton::clicked, this, [=] { parentWidget->close(); });
    connect(ui.btnMinimize, &QPushButton::clicked, this, [=] { parentWidget->showMinimized(); });
    connect(ui.btnMaximize, &QPushButton::clicked, this, [=] {
        parentWidget->showMaximized();
        ui.btnRestore->show();
        ui.btnMaximize->hide();
    });
    connect(ui.btnRestore, &QPushButton::clicked, this, [=] {
        parentWidget->showNormal();
        ui.btnRestore->hide();
        ui.btnMaximize->show();
    });
}

void TitleBar::setTitle(const QString& title) { ui.lblTitle->setText(title); }

void TitleBar::setTitleFont(const QFont& font) { ui.lblTitle->setFont(font); }

void TitleBar::setButtons(TitleButtons button) {
    if (button.testFlag(Restore)) {
        flag.setFlag(Maximize, true);
        flag.setFlag(Restore, false);
    }
    operation(
        button, [](QPushButton* button) { button->show(); },
        [](QPushButton* button) { button->hide(); });
}

QPushButton* TitleBar::getButton(TitleButtons button) {
    if (button == Close)
        return ui.btnClose;
    else if (button == Maximize)
        return ui.btnMaximize;
    else if (button == Minimize)
        return ui.btnMinimize;
    else if (button == Restore)
        return ui.btnRestore;
    return nullptr;
}

void TitleBar::setIconSize(QSize size, TitleButtons button) {
    operation(button, [&size](QPushButton* button) { button->setIconSize(size); });
}

QSize TitleBar::iconSize(TitleButton button) const {
    QSize res;
    if (button != Full)
        const_cast<TitleBar*>(this)->operation(
            button, [&res](QPushButton* button) { res = button->iconSize(); });
    return res;
}

void TitleBar::setIconSize(int w, int h, TitleButtons button) { setIconSize(QSize(w, h), button); }

void TitleBar::setButtonIcon(TitleButton button, QIcon icon) {
    if (button != Full)
        operation(button, [&icon](QPushButton* button) { button->setIcon(icon); });
}

void TitleBar::setButtonSize(QSize size, TitleButtons button) {
    operation(button, [&size](QPushButton* button) { button->setFixedSize(size); });
}

QSize TitleBar::buttonSize(TitleButton button) const {
    QSize res;
    if (button != Full)
        const_cast<TitleBar*>(this)->operation(
            button, [&res](QPushButton* button) { res = button->size(); });
    return res;
}

void TitleBar::setButtonSize(int w, int h, TitleButtons button) {
    setButtonSize(QSize(w, h), button);
}

void TitleBar::setIconColor(const QColor& color, TitleButton button) {
    if (button == Full) {
        setIconColor(color, Close);
        setIconColor(color, Maximize);
        setIconColor(color, Minimize);
        setIconColor(color, Restore);
    }
    else {
        QString path;
        if (button == Close)
            path = ":/xicons/close.svg";
        else if (button == Maximize)
            path = ":/xicons/maximize.svg";
        else if (button == Minimize)
            path = ":/xicons/minimize.svg";
        else if (button == Restore)
            path = ":/xicons/restore.svg";

        operation(button, [&](QPushButton* button) {
            QPixmap  pixmap = QPixmap(path);
            QPainter painter(&pixmap);
            painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
            painter.setBrush(color);
            painter.setPen(color);
            painter.drawRect(pixmap.rect());
            button->setIcon(QIcon(pixmap));
        });
    }
}

void TitleBar::setStyle(QStyle* style, TitleButtons button) {
    operation(button, [=](QPushButton* button) { button->setStyle(style); });
}

void TitleBar::setStyleSheet(const QString& style, TitleButtons button) {
    operation(button, [=](QPushButton* button) { button->setStyleSheet(style); });
}

void TitleBar::addLayout(QLayout* layout) {
    ui.layout->addLayout(layout, ui.layout->rowCount(), 0, 1, ui.layout->columnCount());
}

QWidget* TitleBar::getParentWidget() const { return parentWidget; }

void TitleBar::setParentWidget(QWidget* newParentWidget) { parentWidget = newParentWidget; }

bool TitleBar::eventFilter(QObject* watched, QEvent* event) {
    auto e = static_cast<QMouseEvent*>(event);

    switch (event->type()) {
    case QEvent::MouseButtonPress:
        if (e->button() == Qt::LeftButton) {
            isDragged = true;
            offset    = GLOBAL_POS(e) - parentWidget->frameGeometry().topLeft();
        }
        return true;
    case QEvent::MouseMove:
        if (isDragged)
            parentWidget->move(GLOBAL_POS(e) - offset);
        return true;
    case QEvent::MouseButtonRelease:
        if (e->button() == Qt::LeftButton)
            isDragged = false;
        return true;
    case QEvent::MouseButtonDblClick:
        if (flag.testFlag(Maximize)) {
            if (parentWidget->isMaximized())
                emit ui.btnRestore->clicked();
            else
                emit ui.btnMaximize->clicked();
        }
        return true;
    case QEvent::Paint: {
        QStyleOption opt;

        opt.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
    default:
        break;
    }
    return QWidget::eventFilter(watched, event);
}

template <class Fn, class Fn2>
void TitleBar::operation(TitleButtons button, Fn op, Fn2 op2) {
    if (button.testFlag(Close))
        op(ui.btnClose);
    else
        op2(ui.btnClose);
    if (button.testFlag(Maximize))
        op(ui.btnMaximize);
    else
        op2(ui.btnMaximize);
    if (button.testFlag(Minimize))
        op(ui.btnMinimize);
    else
        op2(ui.btnMinimize);
    if (button.testFlag(Restore))
        op(ui.btnRestore);
    else
        op2(ui.btnRestore);
}
