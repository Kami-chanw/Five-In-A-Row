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

    bool isActive() const;
    void startTimer() { timer.start(); }
    void stopTimer() {
        avatar->setProgress(1);
        currentTime = singleTime;
        timer.stop();
    }

    virtual ~Avatar();

signals:
    void timeout();

private:
    AvatarWidget* avatar;
    QLabel*       lblName;
    QLabel*       lblLeftTime;
    QTime         totalTime;
    int           currentTime;  // ms
    const size_t     singleTime;
    QTimer        timer;

    static constexpr int interval = 10;
};

#endif  // AVATAR_H
