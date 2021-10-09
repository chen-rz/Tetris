#include "widget.h"
#include "ui_widget.h"

#include <iostream>
#include <ctime>
#include <cmath>
#include <QTimer>
#include <QPainter>
#include <QKeyEvent>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

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
            maskT[ii][ij]=0;
            maskC[ii][ij]=QColor(255,255,0);
            gridT[ii][ij]=0;
            gridC[ii][ij]=QColor(255,255,0);
        }
    }

    //参数初始化
    LoLeX = 1;
    LoLeY = 4;
    exaX = 4;
    exaY = 4;
    RowsEliminated = 0;

    historyScore = 0;
    nowScore = 0;

    ifStart = false;
    ifFast = false;
    ifSetout = false;
    ifDisplay = false;

    ifInitial = true;
    vIndex = 1;


    //读取配置信息：
    //旋转方向
    QFile rotFile("../Tetris/config/rotation.txt");
    rotFile.open(QIODevice::ReadOnly);
    QTextStream rotIn(&rotFile);
    rotIn >> xRot;
    rotFile.close();
//    std::cout<<"xRot = "<<xRot<<'\n';

    //加速下落时的速度
    QFile accFile("../Tetris/config/acceleration.txt");
    accFile.open(QIODevice::ReadOnly);
    QTextStream accIn(&accFile);
    accIn >> xAcc;
    accFile.close();
//    std::cout<<"xAcc = "<<xAcc<<'\n';

    //显示即将出现的方块
    QFile upcFile("../Tetris/config/upcoming.txt");
    upcFile.open(QIODevice::ReadOnly);
    QTextStream upcIn(&upcFile);
    upcIn >> xNext;
    upcFile.close();
//    std::cout<<"xNext = "<<xNext<<'\n';

    //按键组合
    QFile keyFile("../Tetris/config/keysettings.txt");
    keyFile.open(QIODevice::ReadOnly);
    QTextStream keyIn(&keyFile);
    keyIn >> xKey;
    keyFile.close();
//    std::cout<<"xKey = "<<xKey<<'\n';

    //显示历史最高分：
    QFile bestFile("../Tetris/config/best_in_history.txt");
    bestFile.open(QIODevice::ReadOnly);
    QTextStream bestIn(&bestFile);
    bestIn >> historyScore;
    bestFile.close();
    ui->txthistory->setText(QString::number(historyScore));

    QMessageBox::information(NULL,"Tetris","★  欢迎！\n\n默认按键组合为：\n← →    左右移动方块\n    ↑      旋转方块\n    ↓      加速/减速下落\n\n默认旋转方向：顺时针方向\n▲  沿任意方向旋转方块时，方块右侧和上方均须有充足的空间！\n\n点击“设置”按钮可更改设置选项\n点击“开始”按钮开始游戏");


    Timer0=new QTimer(this);
    Timer0->setInterval(1000);
    connect(Timer0,&QTimer::timeout,this,&Widget::Timer0_Timer);

    TimerImage=new QTimer(this);
    TimerImage->start(50);
    connect(TimerImage,&QTimer::timeout,this,&Widget::TimerImage_Timer);

    connect(ui->cmdstart,&QPushButton::clicked,this,&Widget::startButton_click);
    connect(ui->settings,&QPushButton::clicked,this,&Widget::showSettings);

    this->setFocus();
}

void Widget::startButton_click(){
    //应用新的设置：
    //旋转方向
    QFile rotFile("../Tetris/config/rotation.txt");
    rotFile.open(QIODevice::ReadOnly);
    QTextStream rotIn(&rotFile);
    rotIn >> xRot;
    rotFile.close();

    //加速下落时的速度
    QFile accFile("../Tetris/config/acceleration.txt");
    accFile.open(QIODevice::ReadOnly);
    QTextStream accIn(&accFile);
    accIn >> xAcc;
    accFile.close();

    //显示即将出现的方块
    QFile upcFile("../Tetris/config/upcoming.txt");
    upcFile.open(QIODevice::ReadOnly);
    QTextStream upcIn(&upcFile);
    upcIn >> xNext;
    upcFile.close();

    //按键组合
    QFile keyFile("../Tetris/config/keysettings.txt");
    keyFile.open(QIODevice::ReadOnly);
    QTextStream keyIn(&keyFile);
    keyIn >> xKey;
    keyFile.close();

    //若为重新开始，则执行初始化
    if(! ifInitial){
        if(IfGameOver()){
            ifFast = false;
            ifInitial = true;
            ifSetout = false;
            ifDisplay = false;
            vIndex = 1;
            Timer0->setInterval(1000*vIndex);
            for(int x=1;x<=10;x++){
                for(int y=1;y<=20;y++){
                    gridT[x][y] = 0;
                    maskT[x][y] = 0;
                }
            }
        }
    }

    //暂停功能
    if(! ifStart){
        ifStart = true;
        Timer0->start();
        ui->cmdstart->setText("暂停");
    }
    else{
        ifStart = false;
        Timer0->stop();
        ui->cmdstart->setText("开始");
    }

}

void Widget::showSettings(){
    //自动暂停游戏
    ifStart = false;
    Timer0->stop();
    ui->cmdstart->setText("开始");

    stui = new Settings();
    stui->setModal(true);
    stui->show();
}

void Widget::keyPressEvent(QKeyEvent *e){
    switch (e->key()) {

    case Qt::Key_Down:
        if(xKey==1 && ifStart){
            if(! ifFast){
                ifFast = true;
                Timer0->setInterval((1000 / xAcc) * vIndex);
            }
            else{
                ifFast = false;
                Timer0->setInterval(1000 * vIndex);
            }
        }
        break;

    case Qt::Key_S:
        if(xKey == 2 && ifStart){
            if(! ifFast){
                ifFast = true;
                Timer0->setInterval((1000 / xAcc) * vIndex);
            }
            else{
                ifFast = false;
                Timer0->setInterval(1000 * vIndex);
            }
        }
        break;

    case Qt::Key_Left:
        if(xKey==1 && ifStart){
            Timer0->stop(); //先关停Timer，防止意外事故
            LeftMove();
            Timer0->start();
        }
        break;

    case Qt::Key_A:
        if(xKey==2 && ifStart){
            Timer0->stop(); //先关停Timer，防止意外事故
            LeftMove();
            Timer0->start();
        }
        break;

    case Qt::Key_Right:
        if(xKey==1 && ifStart){
            Timer0->stop(); //先关停Timer，防止意外事故
            RightMove();
            Timer0->start();
        }
        break;

    case Qt::Key_D:
        if(xKey==2 && ifStart){
            Timer0->stop(); //先关停Timer，防止意外事故
            RightMove();
            Timer0->start();
        }
        break;

    case Qt::Key_Up:
        if(xKey==1 && ifStart){
            Timer0->stop();
            if(xRot == 1) SingleClockwise();
            else SingleCounter();
            Timer0->start();
        }
        break;

    case Qt::Key_W:
        if(xKey==2 && ifStart){
            Timer0->stop();
            if(xRot == 1) SingleClockwise();
            else SingleCounter();
            Timer0->start();
        }
        break;

    default:
        break;

    }
}

void Widget::Timer0_Timer(){

    if(ifInitial == false){ //非初始状态
        if(! ifSink()){ //未抵达终点，下落一行
            Drop();
        }
        else{  //当前方块抵达终点，下一个方块出发
            Terminus();
            DispToAn();
            ConfirmEdge();

            EliminateRow(); //消除被填满的行

            nowScore += RowsEliminated * pow(2, RowsEliminated) + RowsEliminated; //计分，pow幂运算函数
            ui->txtcurrent->setText(QString::number(nowScore));

            if(nowScore > historyScore){
                historyScore = nowScore;
                ui->txthistory->setText(QString::number(historyScore));
            }

            if(nowScore >= 50){
                vIndex = pow(0.9, int(nowScore / 50)); //调节速度
            }

            if(ifFast == true){
                Timer0->setInterval((1000 / xAcc) * vIndex);
            }
            else{
                Timer0->setInterval(1000 * vIndex);
            }

            if(IfGameOver()){ //如果结束
                ui->cmdstart->setText("开始");
                ifStart = false;

                //存储历史最高分
                QFile bestFile("../Tetris/config/best_in_history.txt");
                bestFile.open(QIODevice::WriteOnly);
                QTextStream bestOut(&bestFile);
                bestOut << historyScore;
                bestFile.close();

                Timer0->stop();

                QMessageBox::information(NULL, "Tetris", "游戏结束！\n\n本次得分："+QString::number(nowScore)+"\n历史最高分："+QString::number(historyScore)+"\n\n点击“开始”按钮重新开始游戏");
            }
            else{
                GetLOLEX();
                HereComesMask();
                ifDisplay = false;  //出发后，展示窗为空
            }
        }

        if(ifDisplay == false){
            int WhatsNext;
            qsrand(time(NULL));
            WhatsNext = (qrand()%7) + 1;
            NextShape(WhatsNext);
            NextColor(WhatsNext);
            RndClockwiseTurn();  //任意方向
            ifDisplay = true;
        }
    }

    if(ifInitial == true){  //初始状态（必须放在非初始状态之后！）
        nowScore = 0;
        ui->txtcurrent->setText(QString::number(nowScore));
        int WhatsFirst;
        qsrand(time(NULL));
        WhatsFirst = (qrand()%7) + 1;
        NextShape(WhatsFirst);
        NextColor(WhatsFirst);
        RndClockwiseTurn(); //任意方向
        DispToAn(); //直接出发
        ConfirmEdge();
        GetLOLEX();
        HereComesMask();
        ifInitial = false;
    }
}


void Widget::TimerImage_Timer(){
    ui->txtspeed->setText(QString::number((float)1000/Timer0->interval()));
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
    if(xNext==1){ //选择显示
        for(x=1;x<=4;x++){
            for(y=1;y<=4;y++){
                if(dispT[x][y]==1){
                    painter.setBrush(dispC[x][y]);
                    painter.drawRect(345 + 25 * (x - 1), 50 + 25 * (y - 1), 25, 25);
                }
            }
        }
    }

    //小窗口方格背景
    painter.restore();
    if(xNext==1){ //选择显示
        for(int i=1;i<=100;i+=25){
            for(int j=1;j<=100;j+=25){
                painter.drawRect(345 + i, 50 + j, 25, 25);
            }
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
    QColor c = RndColor();

    if(sqrNo == Osqr){
        dispC[2][2]=c;dispC[2][3]=c;dispC[3][2]=c;dispC[3][3]=c;
    }
    if(sqrNo == Isqr){
        dispC[1][1]=c;dispC[1][2]=c;dispC[1][3]=c;dispC[1][4]=c;
    }
    if(sqrNo == Ssqr){
        dispC[1][2]=c;dispC[1][3]=c;dispC[2][1]=c;dispC[2][2]=c;
    }
    if(sqrNo == Zsqr){
        dispC[1][1]=c;dispC[1][2]=c;dispC[2][2]=c;dispC[2][3]=c;
    }
    if(sqrNo == Lsqr){
        dispC[1][1]=c;dispC[2][1]=c;dispC[3][1]=c;dispC[3][2]=c;
    }
    if(sqrNo == Jsqr){
        dispC[1][2]=c;dispC[2][2]=c;dispC[3][2]=c;dispC[3][1]=c;
    }
    if(sqrNo == Tsqr){
        dispC[1][1]=c;dispC[1][2]=c;dispC[1][3]=c;dispC[2][2]=c;
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
        dispT[2][2]=1;dispT[2][3]=1;dispT[3][2]=1;dispT[3][3]=1;
    }
    if(sqrNo == Isqr){
        dispT[1][1]=1;dispT[1][2]=1;dispT[1][3]=1;dispT[1][4]=1;
    }
    if(sqrNo == Ssqr){
        dispT[1][2]=1;dispT[1][3]=1;dispT[2][1]=1;dispT[2][2]=1;
    }
    if(sqrNo == Zsqr){
        dispT[1][1]=1;dispT[1][2]=1;dispT[2][2]=1;dispT[2][3]=1;
    }
    if(sqrNo == Lsqr){
        dispT[1][1]=1;dispT[2][1]=1;dispT[3][1]=1;dispT[3][2]=1;
    }
    if(sqrNo == Jsqr){
        dispT[1][2]=1;dispT[2][2]=1;dispT[3][2]=1;dispT[3][1]=1;
    }
    if(sqrNo == Tsqr){
        dispT[1][1]=1;dispT[1][2]=1;dispT[1][3]=1;dispT[2][2]=1;
    }
}

//对下一个方块作任意次数顺时针旋转，然后在小窗口展示。第i行变为第n-i+1列，第j列变为第j行
void Widget::RndClockwiseTurn(){
    qsrand(time(NULL));
    int turnTimes = (qrand() % 4) + 1;

    for(int i=0;i<turnTimes;i++){
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
    }
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
    int upBlank=0, downBlank=0, leftBlank=0, rightBlank=0, ceX, ceY;

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
    return (qrand() % (11 - SqrWidth)) + 1;
    //以11减去实际宽度，防止出界
}

//获取下落位置所在的列
void Widget::GetLOLEX(){
    bool isBlocked = false;
    do{
        LoLeX = GetStartColumn(exaX);
        isBlocked = false;
        for(int x = LoLeX;x <= LoLeX+exaX-1;x ++){
            for(int y = 1;y <= exaY;y ++){
                //向当前堆积高度较低的位置投放新方块，防止“落地成盒”
                if(gridT[x][y] + exaT[x - LoLeX + 1][y] == 2){
                    isBlocked = true;
                    break;
                }
            }
            if(isBlocked) break;
        }
    }while(isBlocked);
}

//装入蒙版
//蒙版技术，灵感来源于Adobe Photoshop的图层技术和图层蒙版，可有效分离当前活动的方块和已固定的图形
void Widget::HereComesMask(){

    for(int x=1;x<=10;x++){
        for(int y=1;y<=20;y++){
            maskT[x][y] = 0; //先清空
        }
    }

    //完整地展示在画面顶端
    for(int x = LoLeX;x <= LoLeX+exaX-1;x ++){
        for(int y=1;y <= exaY;y++){
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
        for(int y = LoLeY - exaY + 1;y <= LoLeY;y++){
            if(gridT[x][y+1]+maskT[x][y]==2)
                return true;
        }
    }

    return false;
}

//方块抵达终点，蒙版印入主界面
void Widget::Terminus(){
    for(int x=LoLeX;x <= LoLeX+exaX-1;x++){
        for(int y=LoLeY;y >= LoLeY-exaY+1;y--){ //自底向上，否则出错
            if(maskT[x][y]==1){
                gridT[x][y] = maskT[x][y];
                gridC[x][y] = maskC[x][y];
            }
        }
    }
}

//判断是否游戏结束
bool Widget::IfGameOver(){
    int errCount = 0;
    bool erFlag=false;

    //Lower left X (LoLeX)可以取1~11-exaX，若全都不行则游戏结束
    for(int f = 1;f <= 11-exaX;f ++){
        erFlag=false;
        for(int x = f;x <= f+exaX-1;x++){
            for(int y=1;y<=exaY;y++){
                if(gridT[x][y] + exaT[x-f+1][y] == 2){
                    errCount++;
                    erFlag=true;
                    break;
                }
            }
            if(erFlag) break;
        }
    }

    if(errCount == 11 - exaX) return true;
    else return false;
}

//向左移动
void Widget::LeftMove(){
    if(LoLeX == 1) return; //已到左边界，不可继续
    if(ifSink()) return; //已被固定
    for(int y = LoLeY-exaY+1;y <= LoLeY;y ++){
        if(maskT[LoLeX][y]+gridT[LoLeX-1][y]==2)
            return; //左边有位阻，不可继续
    }
    for(int y = LoLeY - exaY + 1;y <= LoLeY;y++){
        for(int x = LoLeX - 1;x <= LoLeX + exaX - 2;x++){
            maskT[x][y] = maskT[x+1][y];
            maskC[x][y] = maskC[x+1][y];
        }
        maskT[LoLeX + exaX - 1][y]=0;
    }
    LoLeX --;
}

//向右移动
void Widget::RightMove(){
    if(LoLeX == 11 - exaX) return; //已到右边界，不可继续
    if(ifSink()) return; //已被固定
    for(int y = LoLeY - exaY + 1;y<=LoLeY;y++){
        if(maskT[LoLeX + exaX - 1][y]+gridT[LoLeX + exaX][y]==2)
            return; //右边有位阻，不可继续
    }
    for(int y = LoLeY - exaY + 1;y <= LoLeY;y++){
        for(int x = LoLeX + exaX;x >= LoLeX + 1;x--){
            maskT[x][y] = maskT[x-1][y];
            maskC[x][y] = maskC[x-1][y];
        }
        maskT[LoLeX][y]=0;
    }
    LoLeX ++;
}

//顺时针旋转，第i行变为第n-i+1列，第j列变为第j行
void Widget::SingleClockwise(){
    if(LoLeY < exaX || 11 - LoLeX < exaY) return; //旋转后将出界，不可继续
    if(ifSink()) return; //已被固定
    for(int chkX = 1 ;chkX <= exaX;chkX++){
        for(int chkY = 1 ;chkY<= exaY;chkY++){
            if(maskT[LoLeX + chkX - 1][ LoLeY - chkY + 1]+gridT[LoLeX + chkY - 1][LoLeY - exaX + chkX]==2)
                return; //周围有位阻，不可继续
        }
    }

    //旋转
    for(int x=1;x<=4;x++){
        for(int y=1;y<=4;y++){
            medT[5-y][x]=anT[x][y];
            medC[5-y][x]=anC[x][y];
        }
    }
    for(int x=1;x<=4;x++){
        for(int y=1;y<=4;y++){
            anT[x][y]=medT[x][y];
            anC[x][y]=medC[x][y];
        }
    }

    ConfirmEdge(); //裁切边界

    //换蒙版
    for(int x=1;x<=10;x++){
        for(int y=1;y<=20;y++){
            maskT[x][y]=0; //先清空
        }
    }
    for(int x = LoLeX;x <= LoLeX + exaX - 1;x++){
        for(int y = LoLeY - exaY + 1;y <= LoLeY;y++){
            maskT[x][y] = exaT[x - LoLeX + 1][y - LoLeY + exaY];
            maskC[x][y] = exaC[x - LoLeX + 1][y - LoLeY + exaY];
        }
    }

}

//逆时针旋转，第n-i+1列变为第i行，第j行变为第j列
void Widget::SingleCounter(){
    if(LoLeY < exaX || 11 - LoLeX < exaY) return; //旋转后将出界，不可继续
    if(ifSink()) return; //已被固定

    for(int chkX = 1 ;chkX <= exaX;chkX++){
        for(int chkY = 1;chkY <= exaY;chkY++){
            if(maskT[LoLeX + chkX - 1][LoLeY - chkY + 1]+gridT[LoLeX + exaY - chkY][LoLeY - chkX + 1]==2)
                return; //周围有位阻，不可继续
        }
    }

    //旋转
    for(int x=1;x<=4;x++){
        for(int y=1;y<=4;y++){
            medT[y][x] = anT[5-x][y];
            medC[y][x] = anC[5-x][y];
        }
    }
    for(int x=1;x<=4;x++){
        for(int y=1;y<=4;y++){
            anT[x][y] = medT[x][y];
            anC[x][y] = medC[x][y];
        }
    }

    ConfirmEdge(); //裁切边界

    //换蒙版
    for(int x=1;x<=10;x++){
        for(int y=1;y<=20;y++){
            maskT[x][y]=0; //先清空
        }
    }
    for(int x = LoLeX;x <= LoLeX + exaX - 1;x++){
        for(int y = LoLeY - exaY + 1;y <= LoLeY;y++){
            maskT[x][y] = exaT[x - LoLeX + 1][y - LoLeY + exaY];
            maskC[x][y] = exaC[x - LoLeX + 1][y - LoLeY + exaY];
        }
    }

}

//消除被填满的行
void Widget::EliminateRow(){
    RowsEliminated = 0;
    bool fullRow=false;
    int sumRow=0,targetRow;

    do{
        fullRow=false;sumRow = 0;
        for(int y = 1 ;y <= 20;y++){
            for(int x = 1;x <= 10;x++){
                sumRow += gridT[x][y];
            }
            if(sumRow == 10){
                targetRow = y;
                fullRow = true;
                break;
            }
            sumRow = 0;
        }

        if(fullRow){
            if(targetRow>1){
                for(int x=1;x<=10;x++){
                    for(int y=targetRow;y >= 2;y--){
                        gridT[x][y] = gridT[x][y-1];
                        gridC[x][y] = gridC[x][y-1];
                    }
                    gridT[x][1]=0;
                }
            }
            else if(targetRow == 1){
                for(int x=1;x<=10;x++){
                    gridT[x][1] = 0;
                }
            }
            RowsEliminated ++;
        }

    }while(fullRow);

}

Widget::~Widget()
{
    delete Timer0;
    delete TimerImage;
    delete ui;
}
