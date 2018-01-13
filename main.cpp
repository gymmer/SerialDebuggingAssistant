/*
 * 项目名称：SerialDebuggingAssistant
 *
 * 当前版本：1.3(for Linux)
 * 作   者：高岩
 * 完成日期：2014.10.27
 *
 * 取代版本：1.2
 * 原作者 ：高岩
 * 完成日期：2014.10.27
 *
 * 添加功能：略
 * 解决Bug：
 *   1.在Linux下实现了程序的设计。所做改动参考教程。
 ×   2.增加了在QT4版本上，显示汉字的代码
 *
 * 尚存Bug：
 *   1.拉伸程序边框后，内容不伸缩
 *   2.等待添加以下功能：
 *   完善菜单栏的其他功能。如：版本信息，关于我们，使用方法。
 */

#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    MainWindow w;
    w.show();

    return a.exec();
}
