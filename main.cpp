/*
 * 项目名称：SerialDebuggingAssistant
 *
 * 当前版本：1.1
 * 作   者：高岩
 * 完成日期：2014.10.26
 *
 * 取代版本：1.0
 * 原作者 ：高岩
 * 完成日期：2014.10.25
 *
 * 添加功能：略
 * 解决Bug：
 *   1.添加了菜单栏。利用action构建工具栏，删除用toolButton构建工具栏的内容。
 *   2.变更了状态栏的代码。使用纯代码实现菜单栏。删除了ui文件中的Lable：statusBarMes
 * 尚存Bug：
 * 1.拉伸程序边框后，内容不伸缩
 * 2.尚未实现函数：void MainWindow::on_clear_text_pushButton_clicked();其功能为点击“清空数据”按钮，清空textBrowser
 * 3.等待添加以下功能：
 *   读取文件，并进行文件的打开、关闭、保存、另存为等功能。
 *   保存用户设置的参数，每次启动软件后，下拉框直接显示预先设置的参数，而不是每次都被初始化。
 *   添加“恢复默认设置”按钮，点击后所有参数立即恢复为用户预先设置的值。
 *   完善菜单栏的其他功能。
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
