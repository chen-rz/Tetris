#include "widget.h"
#include "settings.h"
#include "ui_widget.h"

#include <QTimer>
#include <QPainter>
#include <ctime>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //数组初始化
    for(int ii=0;ii<5;ii++){
        for(int ij=0;ij<5;ij++){
            dispT[ii][ij]=0;
            dispC[ii][ij]=QColor(255,255,0);
            medT[ii][ij]=0;
            medC[ii][ij]=QColor(255,255,0);
            anT[ii][ij]=0;
            anC[ii][ij]=QColor(255,255,0);
        }
    }
    for(int ii=0;ii<11;ii++){
        for(int ij=0;ij<21;ij++){
            maskT[ii][ij]=0;
            maskC[ii][ij]=QColor(255,255,0);
            gridT[ii][ij]=0;
            gridC[ii][ij]=QColor(255,255,0);
        }
    }

    QTimer* Timer0=new QTimer(this);
    Timer0->start(1000);


    QTimer* TimerImage=new QTimer(this);
    TimerImage->start(50);
    connect(TimerImage,&QTimer::timeout,this,&Widget::TimerImage_Timer);


    connect(ui->settings,&QPushButton::clicked,this,&Widget::showSettings);

}

void Widget::showSettings(){
    stui = new Settings();
    stui->setModal(true);
    stui->show();
}

void Widget::TimerImage_Timer(){
    ui->txtspeed->setText(QString::number((float)1000/50));
    this->repaint();
}

void Widget::paintEvent(QPaintEvent *){
    int x,y;
    QPainter painter(this);
    painter.save();

    //大窗口
    for(x=1;x<=10;x++){
        for(y=1;y<=20;y++){
            if(gridT[x][y]==1){
                painter.setBrush(gridC[x][y]);
                painter.drawRect(30 + 25 * (x - 1), 50 + 25 * (y - 1), 25, 25);
            }
            if(maskT[x][y]==1){
                painter.setBrush(maskC[x][y]);
                painter.drawRect(30 + 25 * (x - 1), 50 + 25 * (y - 1), 25, 25);
            }
        }
    }

    //大窗口方格背景
    painter.restore();
    painter.save();
    for(int i=1;i<=250;i+=25){
        for(int j=1;j<=500;j+=25){
            painter.drawRect(30 + i, 50 + j, 25, 25);
        }
    }

    //小窗口
    for(x=1;x<=4;x++){
        for(y=1;y<=4;y++){
            if(dispT[x][y]==1){
                painter.setBrush(dispC[x][y]);
                painter.drawRect(345 + 25 * (x - 1), 50 + 25 * (y - 1), 25, 25);
            }
        }
    }

    //小窗口方格背景
    painter.restore();
    for(int i=1;i<=100;i+=25){
        for(int j=1;j<=100;j+=25){
            painter.drawRect(345 + i, 50 + j, 25, 25);
        }
    }
}

//随机颜色生成器：
QColor Widget::RndColor(){
    int rr,gg,bb;
    qsrand(time(NULL));

    do{
        rr = qrand()%256;
    }while(rr < 50 || rr > 200); //如果颜色过浅或过深则重新生成，防止干扰视觉。下同
    do{
        gg = qrand()%256;
    }while(gg < 50 || gg > 200);
    do{
        bb = qrand()%256;
    }while(bb < 50 || bb > 200);

    return QColor(rr,gg,bb);
}

//随机生成下一个方块的颜色：
void Widget::NextColor(int sqrNo){
    QColor GiveMeColor = RndColor();

    if(sqrNo == Osqr){
        dispC[2][2]=GiveMeColor;dispC[2][3]=GiveMeColor;dispC[3][2]=GiveMeColor;dispC[3][3]=GiveMeColor;
    }
    if(sqrNo == Isqr){
        dispC[1][1]=GiveMeColor;dispC[1][2]=GiveMeColor;dispC[1][3]=GiveMeColor;dispC[1][4]=GiveMeColor;
    }
    if(sqrNo == Ssqr){
        dispC[1][2]=GiveMeColor;dispC[1][3]=GiveMeColor;dispC[2][1]=GiveMeColor;dispC[2][2]=GiveMeColor;
    }
    if(sqrNo == Zsqr){
        dispC[1][1]=GiveMeColor;dispC[1][2]=GiveMeColor;dispC[2][2]=GiveMeColor;dispC[2][3]=GiveMeColor;
    }
    if(sqrNo == Lsqr){
        dispC[1][1]=GiveMeColor;dispC[2][1]=GiveMeColor;dispC[3][1]=GiveMeColor;dispC[3][2]=GiveMeColor;
    }
    if(sqrNo == Jsqr){
        dispC[1][2]=GiveMeColor;dispC[2][2]=GiveMeColor;dispC[3][2]=GiveMeColor;dispC[3][1]=GiveMeColor;
    }
    if(sqrNo == Tsqr){
        dispC[1][1]=GiveMeColor;dispC[1][2]=GiveMeColor;dispC[1][3]=GiveMeColor;dispC[2][2]=GiveMeColor;
    }

}

//随机生成下一个方块的形状：
void Widget::NextShape(int sqrNo){
    for(int i=1;i<=4;i++){
        for(int j=1;j<=4;j++){
            dispT[i][j]=0;
        }
    }

    if(sqrNo == Osqr){
        dispC[2][2]=1;dispC[2][3]=1;dispC[3][2]=1;dispC[3][3]=1;
    }
    if(sqrNo == Isqr){
        dispC[1][1]=1;dispC[1][2]=1;dispC[1][3]=1;dispC[1][4]=1;
    }
    if(sqrNo == Ssqr){
        dispC[1][2]=1;dispC[1][3]=1;dispC[2][1]=1;dispC[2][2]=1;
    }
    if(sqrNo == Zsqr){
        dispC[1][1]=1;dispC[1][2]=1;dispC[2][2]=1;dispC[2][3]=1;
    }
    if(sqrNo == Lsqr){
        dispC[1][1]=1;dispC[2][1]=1;dispC[3][1]=1;dispC[3][2]=1;
    }
    if(sqrNo == Jsqr){
        dispC[1][2]=1;dispC[2][2]=1;dispC[3][2]=1;dispC[3][1]=1;
    }
    if(sqrNo == Tsqr){
        dispC[1][1]=1;dispC[1][2]=1;dispC[1][3]=1;dispC[2][2]=1;
    }
}

//对下一个方块作任意次数顺时针旋转，然后在小窗口展示。第i行变为第n-i+1列，第j列变为第j行
void Widget::RndClockwiseTurn(){
    int haveTurnedTime=0;
    qsrand(time(NULL));
    int turnTimes = qrand() % 4;
    do{
        for(int x=1;x<=4;x++){
            for(int y=1;y<=4;y++){
                medT[y][5-x] = dispT[x][y];
                medC[y][5-x] = dispC[x][y];
            }
        }
        for(int x=1;x<=4;x++){
            for(int y=1;y<=4;y++){
                dispT[x][y] = medT[x][y];
                dispC[x][y] = medC[x][y];
            }
        }
        haveTurnedTime ++;
    }while(haveTurnedTime < turnTimes);
}

Widget::~Widget()
{
    delete ui;
}
