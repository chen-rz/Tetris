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
            exaT[ii][ij]=0;
            exaC[ii][ij]=QColor(255,255,0);
        }
    }
    for(int ii=0;ii<11;ii++){
        for(int ij=0;ij<21;ij++){
            maskT[ii][ij]=1;
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
    ui->txtspeed->setText(QString::number((float)1000/50/20));
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

//释放展示窗中的方块，准备裁剪边界
void Widget::DispToAn(){
    for(int x=1;x<=4;x++){
        for(int y=1;y<=4;y++){
            anT[x][y] = dispT[x][y];
            anC[x][y] = dispC[x][y];
        }
    }
}

//确认方块的边界
void Widget::ConfirmEdge(){
    int upBlank, downBlank, leftBlank, rightBlank, ceX, ceY;

    //按行正向扫描，记录上方有几行空白
    ceY = 1;
    while(anT[1][ceY]==0 && anT[2][ceY]==0 && anT[3][ceY]==0 && anT[4][ceY]==0){
        upBlank ++;
        ceY ++;
        if(ceY == 5) break;
    }

    //按行反向扫描，记录下方有几行空白
    ceY = 4;
    while(anT[1][ceY]==0 && anT[2][ceY]==0 && anT[3][ceY]==0 && anT[4][ceY]==0){
        downBlank ++;
        ceY --;
        if(ceY == 0) break;
    }

    //按列正向扫描，记录左方有几行空白
    ceX = 1;
    while(anT[ceX][1]==0 && anT[ceX][2]==0 && anT[ceX][3]==0 && anT[ceX][4]==0){
        leftBlank ++;
        ceX ++;
        if(ceX == 5) break;
    }

    //按列反向扫描，记录右方有几行空白
    ceX = 4;
    while(anT[ceX][1]==0 && anT[ceX][2]==0 && anT[ceX][3]==0 && anT[ceX][4]==0){
        rightBlank ++;
        ceX --;
        if(ceX == 0) break;
    }

    //实际尺寸
    exaX = 4 - leftBlank - rightBlank;
    exaY = 4 - upBlank - downBlank;

    //裁去空白
    for(int x=1;x<=exaX;x++){
        for(int y=1;y<=exaY;y++){
            exaT[x][y] = anT[leftBlank + x][upBlank + y];
            exaC[x][y] = anC[leftBlank + x][upBlank + y];
        }
    }

}

//随机生成方块下落的列坐标
int Widget::GetStartColumn(int SqrWidth){
    qsrand(time(NULL));
    return (qrand() % (11 - SqrWidth)) + 1;
    //以11减去实际宽度，防止出界
}

//获取下落位置所在的列
void Widget::GetLOLEX(){
    bool isBlocked = false;
    do{
        LoLeX = GetStartColumn(exaX);
        for(int x = LoLeX;x <= LoLeX+exaX-1;x ++){
            for(int y = 1;y <= exaY;y ++){
                //向当前堆积高度较低的位置投放新方块，防止“落地成盒”
                if(gridT[x][y] + exaT[x - LoLeX + 1][y] == 2){
                    isBlocked = true;
                    break;
                }
            }
        }
    }while(isBlocked);
}

//装入蒙版
void Widget::HereComesMask(){
    for(int x=1;x<=10;x++){
        for(int y=1;y<=20;y++){
            maskT[x][y] = 0; //先清空
        }
    }
    //完整地展示在画面顶端
    for(int x = LoLeX;x <= LoLeX+exaX-1;x ++){
        for(int y=1;y<=exaY;y++){
            maskT[x][y]=exaT[x-LoLeX+1][y];
            maskC[x][y]=exaC[x-LoLeX+1][y];
        }
    }
    LoLeY = exaY;
}

//下落一格
void Widget::Drop(){
    LoLeY ++;
    //将蒙版上的内容整体下移一行
    for(int x=1;x<=10;x++){
        for(int y=20;y>=2;y--){
            maskT[x][y] = maskT[x][y-1];
            maskC[x][y] = maskC[x][y-1];
        }
        maskT[x][1] = 0;
    }
}

//判断能否继续下落
bool Widget::ifSink(){
    if(LoLeY==20) return true; //已到达最后一行

    //蒙版下落一格，每一行是否遇到障碍
    for(int x = LoLeX;x <= LoLeX + exaX - 1;x++){
        for(int y = LoLeY - exaY + 1;y < LoLeY;y++){
            if(gridT[x][y+1]+maskT[x][y]==2)
                return true;
        }
    }

    return false;
}

Widget::~Widget()
{
    delete ui;
}
