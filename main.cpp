/*
 * 项目名称：SerialDebuggingAssistant
 *
 * 当前版本：1.2
 * 作   者：高岩
 * 完成日期：2014.10.27
 *
 * 取代版本：1.1
 * 原作者 ：高岩
 * 完成日期：2014.10.26
 *
 * 添加功能：略
 * 解决Bug：
 *   1.添加了打开文件、发送文件、保存数据功能。并添加上述功能相应的菜单栏和工具栏。
 * 尚存Bug：
 *   1.拉伸程序边框后，内容不伸缩
 *   2.尚未实现函数：void MainWindow::on_clear_text_pushButton_clicked();其功能为点击“清空数据”按钮，清空textBrowser
 *   3.等待添加以下功能：
 *   保存用户设置的参数，每次启动软件后，下拉框直接显示预先设置的参数，而不是每次都被初始化。
 *   添加“恢复默认设置”按钮，点击后所有参数立即恢复为用户预先设置的值。
 *   完善菜单栏的其他功能。如：版本信息，关于我们，使用方法。
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
