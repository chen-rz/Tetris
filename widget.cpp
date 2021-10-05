#include "widget.h"
#include "settings.h"
#include "ui_widget.h"

#include <QTimer>
#include <QPainter>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QTimer* Timer0=new QTimer(this);
    Timer0->start(1000);
    QTimer* TimerImage=new QTimer(this);
    TimerImage->start(50);

    connect(ui->settings,&QPushButton::clicked,this,&Widget::showSettings);

}

void Widget::showSettings(){
    stui = new Settings();
    stui->setModal(true);
    stui->show();
}

void Widget::TimerImage_Timer(int intvl){
    ui->txtspeed->setText(QString::number((float)1000/intvl));
    this->repaint();
}

void Widget::paintEvent(QPaintEvent *){
    int x,y;
    QPainter painter(this);

    //大窗口
    for(x=1;x<=10;x++){
        for(y=1;y<=20;y++){
            if(gridT[x][y]==1 || maskT[x][y]==1){
                painter.drawRect(30 + 25 * (x - 1), 50 + 25 * (y - 1), 25, 25);
            }
        }
    }

    //大窗口方格背景
    for(int i=1;i<=250;i+=25){
        for(int j=1;j<=500;j+=25){
            painter.drawRect(30 + i, 50 + j, 25, 25);
        }
    }

    //小窗口
    for(x=1;x<=4;x++){
        for(y=1;y<=4;y++){
            if(dispT[x][y]==1){
                painter.drawRect(345 + 25 * (x - 1), 50 + 25 * (y - 1), 25, 25);
            }
        }
    }

    //小窗口方格背景
    for(int i=1;i<=100;i+=25){
        for(int j=1;j<=100;j+=25){
            painter.drawRect(345 + i, 50 + j, 25, 25);
        }
    }
}

Widget::~Widget()
{
    delete ui;
}
