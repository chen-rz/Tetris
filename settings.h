#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

public slots:
    void ok_click();
    void cancel_click();

private:
    Ui::Settings *ui;

    int xRot, xAcc, xNext, xKey; //配置信息
};

#endif // SETTINGS_H
