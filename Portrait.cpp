#include"Portrait.h"
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QString>

const int portraitSize=60;
Portrait::Portrait(QWidget* parent) : QWidget(parent) {
    // 载入图片
    image = new QLabel(this);
//    image->setFixedSize(size().shrunkBy(QMargins(2,2,2,2)));
    image->setFixedSize(portraitSize,portraitSize);//设置图片尺寸
    QString path = ":/imgs/R-C.jpg";
    QPixmap pixmap(path);
    pixmap = pixmap.scaled(image->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    int width = image->width();
    int height = image->height();

    QPixmap img(width, height);
    img.fill(Qt::transparent);

    QPainterPath painterPath;
    painterPath.addEllipse(0, 0, width, height);

    QPainter painter(&img);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setClipPath(painterPath);
    painter.drawPixmap(0, 0, width, height, pixmap);
    // 绘制到label
    image->setPixmap(img);
}

const QLabel* Portrait::images() const  {
    return image;
}

