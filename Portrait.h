
#ifndef PORTRAIT_H
#define PORTRAIT_H

#include <QWidget>
#include<QLabel>



class Portrait:public QWidget
{
    Q_OBJECT

public:
    Portrait(QWidget*);

private:
    QLabel*image;

};


#endif // PORTRAIT_H
