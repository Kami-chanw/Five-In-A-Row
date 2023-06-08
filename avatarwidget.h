#ifndef AVATARWIDGET_H
#define AVATARWIDGET_H

#include <QWidget>

class AvatarWidget : public QWidget {
    Q_OBJECT
public:
    AvatarWidget(QWidget* parent = nullptr);

    void   setAvatar(const QImage& avatar);
    void   setProgressBarWidth(const size_t width);
    size_t progressBarWidth() const;

    qreal progress() const;
    void  setProgress(qreal progress);

signals:
    void progressChanged();

protected:
    void paintEvent(QPaintEvent* event);

private:
    QPainterPath arcPath(QRectF rect, int startAngle, int angleLength, int arcHeight);

    QColor interpolateColor(const QColor& color1, const QColor& color2, qreal t);

    static std::vector<QColor> _colorMap;

    QImage _avatar;
    qreal  _progress;
    size_t _progressBarWidth = 20;
};

#endif  // AVATARWIDGET_H
