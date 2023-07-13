#include "avatarwidget.h"
#include <QGridLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QStyle>

AvatarWidget::AvatarWidget(QWidget* parent, LabelPosition pos, const QImage& img,
                           const QString& name, QTime totalTime, size_t singleTime)
    : QWidget(parent), totalTime(totalTime), leftTime(totalTime), currentTime(singleTime),
      singleTime(singleTime), avatarImage(img), currentAvatar(img) {
    // init data
    setMouseTracking(true);

    // init form
    int          labelPosCol = pos == LabelPosition::Left ? 0 : 1;  // 0 for left
    QGridLayout* layout      = new QGridLayout;
    avatar                   = new QWidget;
    avatar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(avatar, 0, 1 - labelPosCol, 2, 1);
    lblName = new QLabel(name);
    lblName->setStyleSheet("font-size:15px;color:#1E1E1E;");
    lblLeftTime = new QLabel(totalTime.toString("mm:ss"));
    lblLeftTime->setProperty("isActive", false);
    lblLeftTime->setStyleSheet("QLabel {"
                               "border-radius:2px;"
                               "letter-spacing:1px;"
                               "padding:-1px 3px -1px 3px;"
                               "}"
                               "QLabel[isActive=\"false\"] {"
                               "background-color:#E9ECEF;"
                               "color:#1E1E1E;"
                               "}"
                               "QLabel[isActive=\"true\"] {"
                               "background-color:#18BC9C;"
                               "color:white;"
                               "}");

    layout->addWidget(lblName, 0, labelPosCol, 1, 1, labelPosCol ? Qt::AlignLeft : Qt::AlignRight);
    layout->addWidget(lblLeftTime, 1, labelPosCol, 1, 1,
                      labelPosCol ? Qt::AlignLeft : Qt::AlignRight);
    layout->setVerticalSpacing(0);
    avatar->setFixedSize(QSize(60, 60));
    setLayout(layout);

    // init timer
    timer.setInterval(interval);
    connect(&timer, &QTimer::timeout, this, [this] {
        currentTime -= interval;
        QTime newTime = leftTime.addMSecs(-interval);
        lblLeftTime->setText(newTime.toString("mm:ss"));
        leftTime.setHMS(newTime.hour(), newTime.minute(), newTime.second(), newTime.msec());
        if (leftTime.isNull() || currentTime == 0) {
            emit timeout();
        }
        if (currentTime < 0.25 * this->singleTime
            && avatarBlinkAnim->state() != QAbstractAnimation::Running) {
            avatarBlinkAnim->start();
        }
        progress = (static_cast<qreal>(currentTime) / this->singleTime);
        update();
    });

    // init color animation of progress bar
    setProperty("progressBarColor", QColor());
    QPropertyAnimation* progOpacityAnim = new QPropertyAnimation(this, "progressBarColor");
    progOpacityAnim->setKeyValues(
        { { 0.0, QColor(0x18, 0xBC, 0x9C, 0) }, { 1.0, QColor(0x18BC9C) } });
    progOpacityAnim->setDuration(200);
    QPropertyAnimation* progColorAnim = new QPropertyAnimation(this, "progressBarColor");
    progColorAnim->setKeyValues({ { 0.0, QColor(0x18BC9C) },
                                  { 0.4, QColor(0x18BC9C) },
                                  { 0.6, QColor(0xF7A215) },
                                  { 0.9, QColor(0xDC3545) },
                                  { 1.0, QColor(0xDC3545) } });
    progColorAnim->setDuration(singleTime - progOpacityAnim->duration());
    progressBarColorAnim = new QSequentialAnimationGroup(this);
    progressBarColorAnim->addAnimation(progOpacityAnim);
    progressBarColorAnim->addAnimation(progColorAnim);

    fadeAnim = new QPropertyAnimation(this, "progressBarColor");
    fadeAnim->setDirection(QAbstractAnimation::Backward);
    fadeAnim->setKeyValues(progOpacityAnim->keyValues());
    fadeAnim->setDuration(progOpacityAnim->duration());
    connect(fadeAnim, &QPropertyAnimation::valueChanged, this, [=] { update(); });

    // init color animation of avatar blink
    setProperty("avatarBlinkColor", QColor());
    avatarBlinkAnim = new QPropertyAnimation(this, "avatarBlinkColor");
    avatarBlinkAnim->setKeyValues({ { 0.0, QColor(0xDC, 0x35, 0x45, 0) },
                                    { 0.5, QColor(0xDC3545) },
                                    { 1.0, QColor(0xDC, 0x35, 0x45, 0) } });
    avatarBlinkAnim->setDuration(700);
    avatarBlinkAnim->setLoopCount(-1);
}

QString AvatarWidget::name() const { return lblName->text(); }

void AvatarWidget::setName(const QString& str) { lblName->setText(str); }

bool AvatarWidget::isActive() const { return timer.isActive(); }

void AvatarWidget::init() {
    leftTime    = totalTime;
    progress    = 1;
    currentTime = singleTime;
    setAvatar(Normal);
    lblLeftTime->setText(leftTime.toString("mm:ss"));
}

void AvatarWidget::startTimer() {
    if (isActive())
        return;
    timer.start();
    currentTime = singleTime;
    lblLeftTime->setProperty("isActive", true);
    lblLeftTime->style()->unpolish(lblLeftTime);
    lblLeftTime->style()->polish(lblLeftTime);
    progressBarColorAnim->start();
}

void AvatarWidget::stopTimer() {
    if (isActive()) {
        progress = 1;
        timer.stop();
        avatarBlinkAnim->stop();
        progressBarColorAnim->stop();
        lblLeftTime->setProperty("isActive", false);
        lblLeftTime->style()->unpolish(lblLeftTime);
        lblLeftTime->style()->polish(lblLeftTime);
        fadeAnim->start();
    }
}

void AvatarWidget::setAvatar(AvatarType type) {
    if (type == AvatarType::Normal)
        currentAvatar = avatarImage;
    else if (type == AvatarType::Dead) {
        currentAvatar = deadImage.isNull() ? QImage(":/imgs/mainwnd_dead.svg") : deadImage;
    }
}

void AvatarWidget::setAvatar(AvatarType type, const QImage& img) {
    if (type == AvatarType::Normal)
        avatarImage = img;
    else if (type == AvatarType::Dead)
        deadImage = img;
}

AvatarWidget::AvatarType AvatarWidget::avatarType() const {
    if (currentAvatar == avatarImage)
        return Normal;
    return Dead;
}

AvatarWidget::~AvatarWidget() {}

void AvatarWidget::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);

    QPainter p(this);
    // draw avatar
    p.setRenderHint(QPainter::Antialiasing, true);
    p.translate(avatar->pos());
    int   diameter = qMin(avatar->rect().width(), avatar->rect().height());
    QRect drawRect(0, 0, diameter, diameter),
        imageRect(0, 0, diameter - progressBarWidth, diameter - progressBarWidth);
    drawRect.moveCenter(avatar->rect().center());
    imageRect.moveCenter(drawRect.center());

    QRegion      imageRegion(imageRect, QRegion::Ellipse);
    QPainterPath clipPath;
    clipPath.addEllipse(imageRect);
    p.setClipPath(clipPath);
    p.drawImage(imageRect, currentAvatar);
    p.setClipping(false);
    if (timer.isActive() || fadeAnim->state() == QAbstractAnimation::Running) {
        QPainterPath outterCircle;
        outterCircle.addRegion(QRegion(drawRect, QRegion::Ellipse).subtracted(imageRegion));
        p.fillPath(outterCircle, Qt::white);
        p.setBrush(property("progressBarColor").value<QColor>());
        p.setPen(p.brush().color());
        if (int angle = progress * 360; angle > 0) {
            QPainterPath arcPath;
            arcPath.moveTo(drawRect.center());
            arcPath.arcTo(drawRect, 90, angle);

            QPainterPath subPath;
            subPath.addEllipse(drawRect.adjusted(progressBarWidth, progressBarWidth,
                                                 -progressBarWidth, -progressBarWidth));
            arcPath -= subPath;
            p.drawPath(arcPath);
        }

        if (avatarBlinkAnim->state() == QAbstractAnimation::Running) {
            p.setPen(Qt::NoPen);
            p.setBrush(property("avatarBlinkColor").value<QColor>());
            p.drawEllipse(imageRect);
        }
    }
}
