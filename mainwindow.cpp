#include "mainwindow.h"
#include "setting.h"
#include "ui_mainwindow.h"
#include "ui_setting.h"
#include <QTimer>
#include <QDebug>
#include <QLabel>
#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("串口调试助手"));

    //设置状态栏
    ui->statusBar->showMessage(tr("欢迎使用串口调试助手！"),4000); //临时消息，显示4s
    QLabel *lable = new QLabel(this);   //显示永久消息
    lable->setText(tr("清华大学·轨道交通控制技术研究中心"));
    lable->setFrameStyle(QFrame::Box | QFrame::Sunken);
    ui->statusBar->addPermanentWidget(lable);

    //设置按钮和下拉框的初始状态：“打开串口”可用，“关闭串口”、“发送数据”、“打开文件”、“发送文件”按钮不可用。下拉框可用,以实现串口参数的设置。
    ui->open_port_pushButton->setEnabled(true);
    ui->close_port_pushButton->setEnabled(false);
    ui->send_data_pushButton->setEnabled(false);
    ui->open_file_pushButton->setEnabled(false);
    ui->send_file_pushButton->setEnabled(false);
    ui->port_name_comboBox->setEnabled(true);
    ui->baud_rate_comboBox->setEnabled(true);
    ui->data_bit_comboBox->setEnabled(true);
    ui->parity_bit_comboBox->setEnabled(true);
    ui->stop_bit_comboBox->setEnabled(true);
    ui->action_open_port->setEnabled(true);
    ui->action_close_port->setEnabled(false);
    ui->action_send_data->setEnabled(false);
    ui->action_open_file->setEnabled(false);
    ui->action_send_file->setEnabled(false);
    ui->action_reset_setting->setEnabled(true);

    //以只读方式读取配置文件，初始化界面各项comboBox
    QFile file("Setting.txt");
    if (!file.open(QIODevice::ReadOnly | QFile::Text))  //无法读取配置文件，提醒用户进行设置
    {
        qDebug() << "\n" << file.errorString() << endl; //用于调试
    }
    else    //读取配置文件成功
    {
        QTextStream in(&file);
        ui->port_name_comboBox->setCurrentIndex(in.readLine().toInt());
        ui->baud_rate_comboBox->setCurrentIndex(in.readLine().toInt());
        ui->data_bit_comboBox->setCurrentIndex(in.readLine().toInt());
        ui->parity_bit_comboBox->setCurrentIndex(in.readLine().toInt());
        ui->stop_bit_comboBox->setCurrentIndex(in.readLine().toInt());

        qDebug() << "\nRead Setting File Success!" << endl; //用于调试
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

//定义槽函数：点击“打开串口”按钮
void MainWindow::on_open_port_pushButton_clicked()
{
    //定义并打开串口
    QString portName = ui->port_name_comboBox->currentText();   //获取串口名
    seriaPort = new Posix_QextSerialPort(portName, QextSerialBase::Polling);  //定义串口，采取查询方式Polling
    seriaPort->open(QIODevice::ReadWrite);  //以读写的方式打开串口

    //设置串口参数。参数由下拉框的选项得到
    //设置波特率
    if (ui->baud_rate_comboBox->currentText() == tr("2400"))
        seriaPort->setBaudRate(BAUD2400);
    else if (ui->baud_rate_comboBox->currentText() == tr("4800"))
        seriaPort->setBaudRate(BAUD4800);
    else if (ui->baud_rate_comboBox->currentText() == tr("9600"))
        seriaPort->setBaudRate(BAUD9600);
    else if (ui->baud_rate_comboBox->currentText() == tr("19200"))
        seriaPort->setBaudRate(BAUD19200);
    else if (ui->baud_rate_comboBox->currentText() == tr("38400"))
        seriaPort->setBaudRate(BAUD38400);
    else if (ui->baud_rate_comboBox->currentText() == tr("57600"))
        seriaPort->setBaudRate(BAUD57600);
    else if (ui->baud_rate_comboBox->currentText() == tr("115200"))
        seriaPort->setBaudRate(BAUD115200);
    //设置数据位
    if (ui->data_bit_comboBox->currentText() == tr("5"))
        seriaPort->setDataBits(DATA_5);
    else if (ui->data_bit_comboBox->currentText() == tr("6"))
        seriaPort->setDataBits(DATA_6);
    else if (ui->data_bit_comboBox->currentText() == tr("7"))
        seriaPort->setDataBits(DATA_7);
    else if (ui->data_bit_comboBox->currentText() == tr("8"))
        seriaPort->setDataBits(DATA_8);
    //设置校验位
    if (ui->parity_bit_comboBox->currentText() == tr("None"))
        seriaPort->setParity(PAR_NONE);
    else if (ui->parity_bit_comboBox->currentText() == tr("Odd"))
        seriaPort->setParity(PAR_ODD);
    else if (ui->parity_bit_comboBox->currentText() == tr("Even"))
        seriaPort->setParity(PAR_EVEN);
    else if (ui->parity_bit_comboBox->currentText() == tr("Mark"))
        seriaPort->setParity(PAR_MARK);
    else if (ui->parity_bit_comboBox->currentText() == tr("Space"))
        seriaPort->setParity(PAR_SPACE);
    //设置停止位
    if (ui->stop_bit_comboBox->currentText() == tr("1"))
        seriaPort->setStopBits(STOP_1);
    else if (ui->stop_bit_comboBox->currentText() == tr("1.5"))
        seriaPort->setStopBits(STOP_1_5);
    else if (ui->stop_bit_comboBox->currentText() == tr("2"))
        seriaPort->setStopBits(STOP_2);

    //为Polling方式创建定时器，以读取缓冲区数据
    QTimer *timer = new QTimer(this);   //创建一个新的定时器
    connect(timer, SIGNAL(timeout), this, SLOT(readPort()));  //信号和槽关联，延迟一段时间后，进行读串口操作
    timer->start(100);  //设置溢出时间为100ms，并开启定时器

    //设置按钮和下拉框的状态：“打开串口”不可用，“关闭串口”、“发送数据”、“打开文件”按钮可用。下拉框不可用。“发送文件”按钮视是否打开了文件而定。
    ui->open_port_pushButton->setEnabled(false);
    ui->close_port_pushButton->setEnabled(true);
    ui->send_data_pushButton->setEnabled(true);
    ui->open_file_pushButton->setEnabled(true);
    //ui->send_file_pushButton->setEnabled(false);
    ui->port_name_comboBox->setEnabled(false);
    ui->baud_rate_comboBox->setEnabled(false);
    ui->data_bit_comboBox->setEnabled(false);
    ui->parity_bit_comboBox->setEnabled(false);
    ui->stop_bit_comboBox->setEnabled(false);
    ui->action_open_port->setEnabled(false);
    ui->action_close_port->setEnabled(true);
    ui->action_send_data->setEnabled(true);
    ui->action_open_file->setEnabled(true);
    //ui->action_send_file->setEnabled(false);
    ui->action_reset_setting->setEnabled(false);

    qDebug() << "\nPort has been opened with following settings:" << endl
             << "Port Name: " << portName << endl
             << "Baud Rate: " << ui->baud_rate_comboBox->currentText() << endl
             << "Data Bits:"  << ui->data_bit_comboBox->currentText() << endl
             << "Parity Bit:" << ui->parity_bit_comboBox->currentText() << endl
             << "Stop Bit :"  << ui->stop_bit_comboBox->currentText() << endl;
}

//定义槽函数：点击“关闭串口”按钮
void MainWindow::on_close_port_pushButton_clicked()
{
    seriaPort->close(); //关闭串口

    //设置按钮和下拉框的状态：“打开串口”可用，“关闭串口”、“发送数据”、“打开文件”、“发送文件”按钮不可用。下拉框可用。
    ui->open_port_pushButton->setEnabled(true);
    ui->close_port_pushButton->setEnabled(false);
    ui->send_data_pushButton->setEnabled(false);
    ui->open_file_pushButton->setEnabled(false);
    ui->send_file_pushButton->setEnabled(false);
    ui->port_name_comboBox->setEnabled(true);
    ui->baud_rate_comboBox->setEnabled(true);
    ui->data_bit_comboBox->setEnabled(true);
    ui->parity_bit_comboBox->setEnabled(true);
    ui->stop_bit_comboBox->setEnabled(true);
    ui->action_open_port->setEnabled(true);
    ui->action_close_port->setEnabled(false);
    ui->action_send_data->setEnabled(false);
    ui->action_open_file->setEnabled(false);
    ui->action_send_file->setEnabled(false);
    ui->action_reset_setting->setEnabled(true);

    qDebug() << "\nPort has been closed!" << endl;  //用于调试
}

//定义槽函数：点击“发送数据”按钮，将行编辑框的数据写入串口
void MainWindow::on_send_data_pushButton_clicked()
{
    seriaPort->write(ui->send_port_lineEdit->text().toLatin1());

    qDebug() << "\nData has been sent!" << endl;    //用于调试
}

//定义槽函数：读取串口数据,在输出窗口显示数据
void MainWindow::readPort()
{
    QByteArray portText = seriaPort->readAll();
    ui->port_receive_textBrowser->insertPlainText(portText);

    qDebug() << "\nData is being read and output..." << endl;   //用于调试
}

//定义槽函数：点击“清空数据”按钮
void MainWindow::on_clear_text_pushButton_clicked()
{
    ui->port_receive_textBrowser->clear();
    ui->port_receive_textBrowser->insertPlainText("Clear Success!\n");

    qDebug() << "\nWindow has been cleared!" << endl;   //用于调试
}

//定义槽函数：点击菜单栏/工具栏的各Action
void MainWindow::on_action_open_port_triggered()
{
    ui->open_port_pushButton->click();
}
void MainWindow::on_action_close_port_triggered()
{
    ui->close_port_pushButton->click();
}
void MainWindow::on_action_send_data_triggered()
{
    ui->send_data_pushButton->click();
}
void MainWindow::on_action_clear_data_triggered()
{
    ui->clear_text_pushButton->click();
}
void MainWindow::on_action_quit_triggered()
{
    ui->quit_pushButton->click();
}
void MainWindow::on_action_open_file_triggered()
{
    ui->open_file_pushButton->click();
}
void MainWindow::on_action_send_file_triggered()
{
    ui->send_file_pushButton->click();
}
void MainWindow::on_action_save_file_triggered()
{
    ui->save_data_pushButton->click();
}

//定义槽函数：点击“打开文件”按钮，分别在行编辑框中显示文件路径和文件内容
void MainWindow::on_open_file_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件");  //弹出“打开文件”对话框，获取文件名
    if (!fileName.isEmpty())
    {
        QFile file(fileName);

        //以只读方式打开文件。若文件无法读取，返回错误信息对话框
        if (!file.open(QIODevice::ReadOnly | QFile::Text))
        {
            QMessageBox::warning(this, tr("读取文件错误！"), tr("无法读取文件%1:n%2").arg(fileName).arg(file.errorString()));
            return;
        }

        //读取文件成功
        //在行编辑框中显示文件路径
        QFileInfo infor(file);
        ui->open_file_lineEdit->setText(infor.absoluteFilePath());
        //在行编辑框中显示文件内容
        QTextStream in(&file);
        ui->file_text_lineEdit->setText(in.readAll());
        //“发送文件”按钮可用
        ui->send_file_pushButton->setEnabled(true);
        ui->action_send_file->setEnabled(true);

        qDebug() << "\nOpen File Success!" << endl << "Path:  " << infor.absoluteFilePath();
    }
}

//定义槽函数：点击“发送文件”按钮，将文件内容发送到串口
void MainWindow::on_send_file_pushButton_clicked()
{
    seriaPort->write(ui->file_text_lineEdit->text().toLatin1());

    qDebug() << "\nFile has been sent!"  << endl;   //用于调试
}

//定义槽函数：点击“保存数据”按钮，将textBrower的内容保存至文件
void MainWindow::on_save_data_pushButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "保存文件");  //获取文件名
    //如果文件名不为空，则保存文件
    if (!fileName.isEmpty())
    {
        QFile file(fileName);

        //以只写方式打开文件。若文件无法读取，则返回错误信息对话框
        if (!file.open(QIODevice::WriteOnly | QFile::Text))
        {
            QMessageBox::warning(this, tr("保存文件错误！"), tr("无法保存文件%1:n%2").arg(fileName).arg(file.errorString()));
            return;
        }

        //读取文件成功
        QTextStream out(&file);
        out << ui->port_receive_textBrowser->toPlainText();

        QFileInfo infor(file);  //用于测试
        qDebug() << "\nSave File Success!" << endl << "Path: " << infor.absolutePath();
    }
}

//定义槽函数：点击“默认设置”action,弹出“用户设置”对话框
void MainWindow::on_action_default_setting_triggered()
{
    setting *setwindow = new setting(this);
    setwindow->show();
}

//定义槽函数：点击“恢复默认”action
void MainWindow::on_action_reset_setting_triggered()
{
    QFile file("Setting.txt");

    //以只读方式打开配置文件
    if (!file.open(QIODevice::ReadOnly | QFile::Text))  //无法读取配置文件，提醒用户进行设置
    {
        qDebug() << "\n" << file.errorString() << endl; //用于调试
        return;
    }
    else    //成功读取配置文件，更改界面各项comboBox
    {
        QTextStream in(&file);
        ui->port_name_comboBox->setCurrentIndex(in.readLine().toInt());
        ui->baud_rate_comboBox->setCurrentIndex(in.readLine().toInt());
        ui->data_bit_comboBox->setCurrentIndex(in.readLine().toInt());
        ui->parity_bit_comboBox->setCurrentIndex(in.readLine().toInt());
        ui->stop_bit_comboBox->setCurrentIndex(in.readLine().toInt());

        qDebug() << "\nSetting has been reset!" << endl; //用于调试
    }
}
