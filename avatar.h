#ifndef AVATAR_H
#define AVATAR_H

#include <QGridLayout>
#include <QImage>
#include <QLabel>
#include <QString>
#include <QTime>
#include <QTimer>
#include <QWidget>

#include "avatarwidget.h"

class Avatar : public QWidget {
    Q_OBJECT

public:
    enum class LabelPosition { Left, Right };
    Avatar(QWidget* parent, LabelPosition pos, const QImage& img, const QString& name = "",
           QTime totalTime = QTime(), size_t singleTime = 0);

    QString name() const;
    void    setName(const QString& str);
    bool    isActive() const;
    void    resetLeftTime();
    void startTimer();
    void stopTimer();

    virtual ~Avatar();

signals:
    void timeout();

private:
    AvatarWidget* avatar;
    QLabel*       lblName;
    QLabel*       lblLeftTime;
    const QTime   totalTime;
    QTime         leftTime;
    int           currentTime;  // ms
    const size_t  singleTime;
    QTimer        timer;

    static constexpr int interval = 10;
};

#endif  // AVATAR_H
