#ifndef AVATARWIDGET_H
#define AVATARWIDGET_H

#include <QImage>
#include <QLabel>
#include <QPropertyAnimation>
#include <QString>
#include <QTime>
#include <QTimer>
#include <QWidget>
#include <vector>

class AvatarWidget : public QWidget {
    Q_OBJECT
public:
    enum LabelPosition { Left, Right };
    enum AvatarType { Normal, Dead };
    Q_ENUM(LabelPosition);
    Q_ENUM(AvatarType);

    AvatarWidget(QWidget* parent, LabelPosition pos, const QImage& img, const QString& name = "",
                 QTime totalTime = QTime(), size_t singleTime = 0);

    QString name() const;
    void    setName(const QString& str);

    bool isActive() const;
    void init();
    void startTimer();
    void stopTimer();

    void       setAvatar(AvatarType type);
    void       setAvatar(AvatarType type, const QImage& img);
    AvatarType avatarType() const;

    virtual ~AvatarWidget();

signals:
    void timeout();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QWidget*     avatar;
    QLabel*      lblName;
    QLabel*      lblLeftTime;
    const QTime  totalTime;
    QTime        leftTime;
    int          currentTime;  // ms
    const size_t singleTime;
    QTimer       timer;

    qreal                      progress;
    QImage                     avatarImage;
    QImage                     deadImage;
    QImage                     currentAvatar;
    QSequentialAnimationGroup* progressBarColorAnim;
    QPropertyAnimation*        fadeAnim;
    QPropertyAnimation*        avatarBlinkAnim;

    static constexpr int progressBarWidth = 5;
    static constexpr int interval         = 10;  // ms
};

#endif  // AVATARWIDGET_H
