#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "win_qextserialport.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_open_port_pushButton_clicked(); //声明槽函数：点击“打开串口”按钮
    void on_close_port_pushButton_clicked();    //声明槽函数：点击“关闭串口”按钮
    void on_send_data_pushButton_clicked(); //声明槽函数：点击“发送数据”按钮，将行编辑框的数据写入串口
    void readPort();    //声明槽函数：读取串口数据,在输出窗口显示数据
    void on_clear_text_pushButton_clicked();    //声明槽函数：点击“清空数据”按钮
    void on_open_file_pushButton_clicked(); //声明槽函数，点击“打开文件”按钮
    void on_send_file_pushButton_clicked(); //声明槽函数，点击“发送文件”按钮
    void on_save_data_pushButton_clicked(); //声明槽函数，点击“保存数据”按钮

    //声明槽函数：点击菜单栏/工具栏的各Action
    void on_action_open_port_triggered();
    void on_action_close_port_triggered();
    void on_action_send_data_triggered();
    void on_action_clear_data_triggered();
    void on_action_quit_triggered();
    void on_action_open_file_triggered();
    void on_action_send_file_triggered();
    void on_action_save_file_triggered();

    void on_action_default_setting_triggered();

    void on_action_reset_setting_triggered();

private:
    Ui::MainWindow *ui;
    Win_QextSerialPort *seriaPort;   //声明一个串口
};

#endif // MAINWINDOW_H
