#include "avatar.h"

Avatar::Avatar(QWidget* parent, LabelPosition pos, const QImage& img, const QString& name,
               QTime totalTime, size_t singleTime)
    : QWidget(parent), totalTime(totalTime), leftTime(totalTime), currentTime(singleTime),
      singleTime(singleTime) {
    // init form
    int labelPosCol = pos == LabelPosition::Left ? 0 : 1;  // 0 for left

    QGridLayout* layout = new QGridLayout;
    avatar              = new AvatarWidget;
    avatar->setAvatar(img);
    avatar->setProgress(1);
    avatar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(avatar, 0, 1 - labelPosCol, 2, 1);
    lblName = new QLabel(name);
    lblName->setStyleSheet("font-size:14px;");
    lblLeftTime = new QLabel(totalTime.toString("mm:ss"));
    lblLeftTime->setStyleSheet("background-color:#18BC9C;"
                               "color:white;"
                               "border-radius:2px;"
                               "letter-spacing:1px;"
                               "padding:5px 3px 5px 3px;");

    avatar->setProgressBarWidth(5);

    layout->addWidget(lblName, 0, labelPosCol, 1, 1, labelPosCol ? Qt::AlignLeft : Qt::AlignRight);
    layout->addWidget(lblLeftTime, 1, labelPosCol, 1, 1,
                      labelPosCol ? Qt::AlignLeft : Qt::AlignRight);
    int avatarHeight = 60;
    //        lblName->geometry().height() + lblLeftTime->geometry().height() +
    //        layout->verticalSpacing();
    avatar->setFixedSize(QSize(avatarHeight, avatarHeight));
    setLayout(layout);
    // init data

    timer.setInterval(interval);
    connect(&timer, &QTimer::timeout, this, [=] {
        this->currentTime -= interval;
        QTime newTime = this->leftTime.addMSecs(-interval);
        lblLeftTime->setText(newTime.toString("mm:ss"));
        this->leftTime.setHMS(newTime.hour(), newTime.minute(), newTime.second(), newTime.msec());
        if (this->leftTime.isNull() || this->currentTime <= 50) {
            timer.stop();
            emit timeout();
        }
        avatar->setProgress(static_cast<qreal>(currentTime) / singleTime);
    });
}

QString Avatar::name() const { return lblName->text(); }

void Avatar::setName(const QString& str) { lblName->setText(str); }

bool Avatar::isActive() const { return timer.isActive(); }

void Avatar::resetLeftTime() {
    leftTime = totalTime;
    avatar->setProgress(1);
    currentTime = singleTime;
    lblLeftTime->setText(leftTime.toString("mm:ss"));
}

void Avatar::startTimer() { timer.start(); }

void Avatar::stopTimer() {
    avatar->setProgress(1);
    currentTime = singleTime;

    timer.stop();
}

Avatar::~Avatar() {}
