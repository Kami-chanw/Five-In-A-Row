#include "avatarwidget.h"
#include "qpainterpath.h"
#include <QStylePainter>

AvatarWidget::AvatarWidget(QWidget* parent) : QWidget(parent) {
    connect(this, &AvatarWidget::progressChanged, this, [&] { update(); });
}

void   AvatarWidget::setAvatar(const QImage& avatar) { _avatar = avatar; }
void   AvatarWidget::setProgressBarWidth(const size_t width) { _progressBarWidth = width; }
size_t AvatarWidget::progressBarWidth() const { return _progressBarWidth; }

qreal AvatarWidget::progress() const { return _progress; }
void AvatarWidget:: setProgress(qreal progress) {
     assert(0 <= progress && progress <= 1.0);
     _progress = progress;
     update();
}

void AvatarWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QStylePainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    QRect drawRect = rect();
    int   radius   = qMin(rect().width(), rect().height());
    drawRect.setSize(QSize(radius, radius));
    drawRect.moveCenter(rect().center());
    QRect imageRect = drawRect;
    imageRect.setSize(
        QSize(drawRect.width() - _progressBarWidth, drawRect.height() - _progressBarWidth));
    imageRect.moveCenter(drawRect.center());
    QRegion imageRegion(imageRect, QRegion::Ellipse);
    QPainterPath clipPath;
    clipPath.addEllipse(imageRect);
    p.setClipPath(clipPath);
    p.drawImage(imageRect, _avatar);
    p.setClipping(false);

    QPainterPath outterCircle;
    outterCircle.addRegion(QRegion(drawRect, QRegion::Ellipse).subtracted(imageRegion));
    p.fillPath(outterCircle, Qt::white);

    int  angle        = progress() * 360;
    auto currentIndex = (1.0 - progress()) * _colorMap.size();
    p.setBrush(interpolateColor(
        _colorMap[currentIndex],
        _colorMap[currentIndex + 1 > _colorMap.size() ? currentIndex : currentIndex + 1],
        currentIndex - int(currentIndex)));
    p.setPen(Qt::NoPen);
    if (angle > 0)
        p.drawPath(arcPath(drawRect, 90, angle, _progressBarWidth));
}

QPainterPath AvatarWidget::arcPath(QRectF rect, int startAngle, int angleLength, int arcHeight) {
    QPainterPath path;
    path.moveTo(rect.center());
    path.arcTo(rect, startAngle, angleLength);

    QPainterPath subPath;
    subPath.addEllipse(rect.adjusted(arcHeight, arcHeight, -arcHeight, -arcHeight));
    path -= subPath;
    return path;
}

QColor AvatarWidget:: interpolateColor(const QColor& color1, const QColor& color2, qreal t) {
    int r = color1.red() * (1 - t) + color2.red() * t;
    int g = color1.green() * (1 - t) + color2.green() * t;
    int b = color1.blue() * (1 - t) + color2.blue() * t;

    return QColor(r, g, b);
}

std::vector<QColor> AvatarWidget::_colorMap{ QColor(0x18, 0xBC, 0x9C), QColor(0x18, 0xBC, 0x9C),
                                             QColor(0x18, 0xBC, 0x9C), QColor(0xF7, 0xA2, 0x15),
                                             QColor(0xDC, 0x35, 0x45) };
