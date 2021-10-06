#ifndef WIDGET_H
#define WIDGET_H

//7种方块编号：
#define Osqr 1
#define Isqr 2
#define Ssqr 3
#define Zsqr 4
#define Lsqr 5
#define Jsqr 6
#define Tsqr 7

#include <QWidget>
#include <QVector>
#include "settings.h"

namespace Ui {
    class Widget;
}

class Widget : public QWidget{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    int xRot, xAcc, xNext, xKey; //配置信息

    int dispT[5][5]; //在小窗口展示的下一个方块，形状信息
    QColor dispC[5][5]; //在小窗口展示的下一个方块，颜色信息
    int medT[5][5]; //中转存储形状信息，0为空，1为满
    QColor medC[5][5]; //中转存储颜色信息
    int anT[5][5]; //下一个方块形状，0为空，1为满
    QColor anC[5][5]; //下一个方块颜色
    QVector<int> exaT(); //确认过边界的方块形状，0为空，1为满
    QVector<QColor> exaC(); //确认过边界的方块颜色
    int maskT[11][21]; //蒙版：正在移动的方块形状信息，0为空，1为满
    QColor maskC[11][21]; //蒙版：正在移动的方块颜色信息
    int gridT[11][21]; //主界面方格形状
    QColor gridC[11][21]; //主界面方格颜色

    int LoLeX, LoLeY; //确认过边界的方块，左下角的方格在gridT中的坐标
    int exaX, exaY; //方块裁去空白后的尺寸

    int RowsEliminated; //一次消除的行数


public slots:
    void showSettings();
    void TimerImage_Timer();
    void paintEvent(QPaintEvent*);


private:
    Ui::Widget *ui;
    Settings *stui;

    bool ifStart; //运行或暂停
    bool ifFast; //是否处于加速下落状态
    bool ifInitial; //是否初始化
    bool ifSetout; //前一个方块是否出发
    bool ifDisplay; //是否已展示下一个方块

    double vIndex; //速度值参数，得分每增加2000分，vIndex打九折
    int historyScore; //历史最高分
    int nowScore; //当前得分

    QColor RndColor();
    void NextColor(int sqrNo);
    void NextShape(int sqrNo);
    void RndClockwiseTurn();
};

#endif // WIDGET_H
