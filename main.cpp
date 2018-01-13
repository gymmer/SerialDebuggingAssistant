/*
 * 项目名称：SerialDebuggingAssistant
 *
 * 当前版本：1.0
 * 作   者：高岩
 * 完成日期：2014.10.25
 *
 * 取代版本：无
 * 原作者 ：无
 * 完成日期：无
 *
 * 添加功能：略
 * 解决Bug：无
 * 尚存Bug：
 * 1.拉伸程序边框后，内容不伸缩
 * 2.尚未实现函数：void MainWindow::on_clear_text_pushButton_clicked();其功能为点击“清空数据”按钮，清空textBrowser
 */

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
