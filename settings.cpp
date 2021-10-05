#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Widget);

    connect(ui->buttonBox,&QDialogButtonBox::accepted,this,&Settings::ok);
    connect(ui->buttonBox,&QDialogButtonBox::rejected,this,&Settings::cancel);

}

void Settings::ok(){
    //写入设置

    //关闭窗口
    this->hide();
}

void Settings::cancel(){

    //关闭窗口
    this->hide();
}

Settings::~Settings()
{
    delete ui;
}
