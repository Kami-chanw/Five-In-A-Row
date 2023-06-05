
#ifndef REVERSETIME_H
#define REVERSETIME_H

#include<QLCDNumber>
#include<QTime>
#include<QPushButton>

class reversetime:public QWidget
{
    Q_OBJECT

public :
    reversetime(QWidget* parent=nullptr);
    void onTimeout();
private slots:
    void clickOnButton();
private:
    QLCDNumber *lcd;
    QTimer *timer;
    int minute=0,second=60;
    QPushButton *startbutton;

};

#endif // REVERSETIME_H
