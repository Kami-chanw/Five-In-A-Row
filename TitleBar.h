#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
class TitleBar : public QWidget {
    Q_OBJECT

public:
    enum TitleButton {
        Close    = 0x1,
        Minimize = 0x2,
        Maximize = 0x4,
        Full     = Close | Minimize | Maximize,
        Restore  = 0x8
    };
    Q_DECLARE_FLAGS(TitleButtons, TitleButton)
    Q_FLAG(TitleButtons)

    explicit TitleBar(QWidget* parentWidget, TitleButtons button = Full, QWidget* parent = nullptr);

    void         setTitle(const QString& title);
    void         setTitleFont(const QFont& font);
    void         setButtons(TitleButtons button);
    QPushButton* getButton(TitleButtons button);
    void         setIconSize(QSize size, TitleButtons button = Full);
    QSize        iconSize(TitleButton button) const;
    void         setIconSize(int w, int h, TitleButtons button = Full);
    void         setButtonIcon(TitleButton button, QIcon icon);
    void         setButtonSize(QSize size, TitleButtons button = Full);
    QSize        buttonSize(TitleButton button) const;
    void         setButtonSize(int w, int h, TitleButtons button = Full);
    void         setIconColor(const QColor& color,
                              TitleButton   button = Full);  // only used for default icon resources
    void         setStyle(QStyle* style, TitleButtons button);
    using QWidget::setStyle;
    void setStyleSheet(const QString& style, TitleButtons button);
    using QWidget::setStyleSheet;

    QWidget* getParentWidget() const;
    void     setParentWidget(QWidget* newParentWidget);

    void addLayout(QLayout* layout);

private:
    class TitleBarUi {
    public:
        QGridLayout* layout;
        QLabel*      lblTitle;
        QSpacerItem* horizontalSpacer;
        QPushButton* btnMinimize;
        QPushButton* btnMaximize;
        QPushButton* btnRestore;
        QPushButton* btnClose;

        void setupUi(QWidget* title);
    };
    TitleBarUi ui;

private:
    template <class Fn, class Fn2 = void(QPushButton*)>
    void operation(
        TitleButtons button, Fn op, Fn2 op2 = [](QPushButton*) {});

    TitleButtons flag      = Full;
    bool         isDragged = false;
    QPoint       offset;
    QWidget*     parentWidget;

    // QObject interface
public:
    bool eventFilter(QObject* watched, QEvent* event) override;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(TitleBar::TitleButtons)

#endif  // TITLEBAR_H
