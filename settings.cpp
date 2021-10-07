#include "settings.h"
#include "ui_settings.h"

#include <QFile>
#include <QTextStream>

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Widget);

    //读取配置信息：
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

    if(xRot == 1){
        ui->optclock->setChecked(true);
    }
    else{
        ui->optcounter->setChecked(true);
    }

    ui->txtaccel->setText(QString::number(xAcc));

    if(xNext == 1){
        ui->checknext->setChecked(true);
    }
    else{
        ui->checknext->setChecked(false);
    }

    if(xKey == 1){
        ui->optarrow->setChecked(true);
    }
    else{
        ui->optletter->setChecked(true);
    }


    connect(ui->buttonBox,&QDialogButtonBox::accepted,this,&Settings::ok_click);
    connect(ui->buttonBox,&QDialogButtonBox::rejected,this,&Settings::cancel_click);

}

void Settings::ok_click(){
    //写入设置

    //存储配置信息：
    //旋转方向：1 = 顺时针，2 = 逆时针
    QFile rotFile("../Tetris/config/rotation.txt");
    rotFile.open(QIODevice::WriteOnly);
    QTextStream rotOut(&rotFile);
    if(ui->optclock->isChecked()){
        rotOut << "1";
    }
    else{
        rotOut << "2";
    }
    rotFile.close();

    //加速下落时的速度：速度值
    QFile accFile("../Tetris/config/acceleration.txt");
    accFile.open(QIODevice::WriteOnly);
    QTextStream accOut(&accFile);
    accOut << ui->txtaccel->text();
    accFile.close();

    //显示即将出现的方块：0 = 否，1 = 是
    QFile upcFile("../Tetris/config/upcoming.txt");
    upcFile.open(QIODevice::WriteOnly);
    QTextStream upcOut(&upcFile);
    if(ui->checknext->isChecked()){
        upcOut << "1";
    }
    else{
        upcOut << "0";
    }
    upcFile.close();

    //按键组合：1 = 方向键，2 = WASD键
    QFile keyFile("../Tetris/config/keysettings.txt");
    keyFile.open(QIODevice::WriteOnly);
    QTextStream keyOut(&keyFile);
    if(ui->optarrow->isChecked()){
        keyOut << "1";
    }
    else{
        keyOut << "2";
    }
    keyFile.close();

    //关闭窗口
    this->hide();
}

void Settings::cancel_click(){
    //保留原有的配置信息，还原各选项值
    if(xRot == 1){
        ui->optclock->setChecked(true);
    }
    else{
        ui->optcounter->setChecked(true);
    }

    ui->txtaccel->setText(QString::number(xAcc));

    if(xNext == 1){
        ui->checknext->setChecked(true);
    }
    else{
        ui->checknext->setChecked(false);
    }

    if(xKey == 1){
        ui->optarrow->setChecked(true);
    }
    else{
        ui->optletter->setChecked(true);
    }

    //关闭窗口
    this->hide();
}

Settings::~Settings()
{
    delete ui;
}
