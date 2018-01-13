#include "setting.h"
#include "ui_setting.h"
#include <QMessageBox>
#include <QDebug>
#include <QFile>

setting::setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setting)
{
    ui->setupUi(this);
    setWindowTitle("用户设置");

    //以只读方式打开配置文件Setting.txt，初始化各ComboBox
    QFile file("Setting.txt");
    if (!file.open(QIODevice::ReadOnly | QFile::Text))  //无法读取配置文件，提醒用户进行设置
    {
        qDebug() << "\n" << file.errorString() << endl; //用于调试
    }
    else    //读取配置文件成功
    {
        QTextStream in(&file);
        ui->set_name_comboBox->setCurrentIndex(in.readLine().toInt());
        ui->set_baud_comboBox->setCurrentIndex(in.readLine().toInt());
        ui->set_data_comboBox->setCurrentIndex(in.readLine().toInt());
        ui->set_parity_comboBox->setCurrentIndex(in.readLine().toInt());
        ui->set_stop_comboBox->setCurrentIndex(in.readLine().toInt());
        file.close();

        qDebug() << "\nRead Setting File Success!" << endl; //用于调试
    }
}

setting::~setting()
{
    delete ui;
}

//定义槽函数：点击“保存”按钮，保存用户设置的“默认设置”至配置文件Setting.txt
void setting::on_save_setting_pushButton_clicked()
{
    QFile file("Setting.txt");

    //以只写方式打开文件。若文件不存在，那么创建文件Setting.txt
    if (!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        qDebug() << endl << file.errorString();
    }

    //保存用户设置的“默认设置”至配置文件
    QTextStream out(&file);
    out << ui->set_name_comboBox->currentIndex() << endl
        << ui->set_baud_comboBox->currentIndex() << endl
        << ui->set_data_comboBox->currentIndex() << endl
        << ui->set_parity_comboBox->currentIndex() << endl
        << ui->set_stop_comboBox->currentIndex() << endl;
    file.close();
    close();    //关闭“用户设置”对话框

    qDebug() << "\nSave Setting Success!" << endl;
}

//定义槽函数：点击“关闭”按钮，取消已进行的配置，关闭“用户设置”对话框
void setting::on_cancel_setting_pushButton_clicked()
{
    close();

    qDebug() << "\nSetting has been canceled!" << endl;
}
