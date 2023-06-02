
#include "reversetime.h"
#include <QTimer>
#include <QWidget>
#include<QMessageBox>

reversetime::reversetime(QWidget *parent)
    : QWidget(parent)
{
    lcd=new QLCDNumber();
    lcd->setDigitCount(5);
    lcd->setMode(QLCDNumber::Dec);
    lcd->setSegmentStyle(QLCDNumber::Flat);


    timer=new QTimer();
    timer->setInterval(1000);
    timer->start();


    startbutton=new QPushButton("开始工作");
    connect(startbutton,SIGNAL(clicked(bool)),this,SLOT(clickOnButton()));


    this->resize(400,200);
    this->setWindowFlag(Qt::WindowStaysOnTopHint);
    //this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
}


void reversetime::onTimeout(){
    QString s_minute=QString::number(minute);
    QString s_second=QString::number(second);
    if (s_minute.length()==1)   s_minute="0"+s_minute;
    if(s_second.length()==1)    s_second="0"+s_second;
    QString disp=s_minute+":"+s_second;
    lcd->display(disp);
    //QTime time = QTime::currentTime();
    //lcd->display(time.toString("mm:ss"));
    if((minute!=0)|(second!=0)){
        if(second==0){
            second=59;
            minute--;}
        else
            second--;
    }
    else
        lcd->setStyleSheet("background-color:red");
    //lcd->display(QTime::currentTime().toString(QString::number(minute)+":"+QString::number(second)));

    if((minute!=0)&&(second!=0))
         QMessageBox::warning(this,"Title","Error Message");
}

void reversetime::clickOnButton(){
    this->minute=0;
    this->second=60;
    disconnect(timer,SIGNAL(timeout()),this,SLOT(onTimeout()));
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimeout()));
    this->lcd->setStyleSheet("background-color:white");
}
