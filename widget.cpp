#include "widget.h"
#include "ui_widget.h"

#include <iostream>
#include <ctime>
#include <QTimer>
#include <QPainter>
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
    int tmp_best;
    bestIn >> tmp_best;
    bestFile.close();
    ui->txthistory->setText(QString::number(tmp_best));


    QMessageBox::information(NULL,"Tetris","★  欢迎！\n\n▲  运行此程序时，须使用英文输入法！\n\n默认按键组合为：\n← →    左右移动方块\n    ↑      旋转方块\n    ↓      加速/减速下落\n\n默认旋转方向：顺时针方向\n▲  沿任意方向旋转方块时，方块右侧和上方均须有充足的空间！\n\n点击“设置”按钮可更改设置选项\n点击“开始”按钮或按Enter键开始");



    Timer0=new QTimer(this);
    Timer0->start(1000);


    TimerImage=new QTimer(this);
    TimerImage->start(50);
    connect(TimerImage,&QTimer::timeout,this,&Widget::TimerImage_Timer);


    connect(ui->settings,&QPushButton::clicked,this,&Widget::showSettings);

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
//蒙版技术，灵感来源于Adobe Photoshop的图层技术和图层蒙版，可有效分离当前活动的方块和已固定的图形
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

    for(int f = 1;f <= 11-exaX;f ++){
        for(int x = f;x <= f+exaX-1;x++){
            for(int y=1;y<=exaY;y++){
                if(gridT[x][y] + exaT[x-f+1][y] == 2){
                    errCount++;
                    erFlag=true;
                    break;
                }
            }
            if(erFlag){
                erFlag=false;
                break;
            }
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
