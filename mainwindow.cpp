#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDebug>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("串口调试助手");

    //设置状态栏
    ui->statusBar->showMessage("欢迎使用串口调试助手！",4000); //临时消息，显示4s
    QLabel *lable = new QLabel(this);   //显示永久消息
    lable->setText("清华大学·轨道交通控制技术研究中心");
    lable->setFrameStyle(QFrame::Box | QFrame::Sunken);
    ui->statusBar->addPermanentWidget(lable);

    //设置按钮和下拉框的初始状态：“打开串口”可用，“关闭串口”和“发送数据”按钮不可用。下拉框可用,以实现串口参数的设置。
    ui->open_port_pushButton->setEnabled(true);
    ui->close_port_pushButton->setEnabled(false);
    ui->send_data_pushButton->setEnabled(false);
    ui->port_name_comboBox->setEnabled(true);
    ui->baud_rate_comboBox->setEnabled(true);
    ui->data_bit_comboBox->setEnabled(true);
    ui->parity_bit_comboBox->setEnabled(true);
    ui->stop_bit_comboBox->setEnabled(true);
    ui->action_open_port->setEnabled(true);
    ui->action_close_port->setEnabled(false);
    ui->action_send_data->setEnabled(false);
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
    seriaPort = new Win_QextSerialPort(portName, QextSerialBase::Polling);  //定义串口，采取查询方式Polling
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

    //设置按钮和下拉框的状态：“打开串口”不可用，“关闭串口”和“发送数据”按钮可用。下拉框不可用。
    ui->open_port_pushButton->setEnabled(false);
    ui->close_port_pushButton->setEnabled(true);
    ui->send_data_pushButton->setEnabled(true);
    ui->port_name_comboBox->setEnabled(false);
    ui->baud_rate_comboBox->setEnabled(false);
    ui->data_bit_comboBox->setEnabled(false);
    ui->parity_bit_comboBox->setEnabled(false);
    ui->stop_bit_comboBox->setEnabled(false);
    ui->action_open_port->setEnabled(false);
    ui->action_close_port->setEnabled(true);
    ui->action_send_data->setEnabled(true);

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

    //设置按钮和下拉框的状态：“打开串口”可用，“关闭串口”和“发送数据”按钮不可用。下拉框可用。
    ui->open_port_pushButton->setEnabled(true);
    ui->close_port_pushButton->setEnabled(false);
    ui->send_data_pushButton->setEnabled(false);
    ui->port_name_comboBox->setEnabled(true);
    ui->baud_rate_comboBox->setEnabled(true);
    ui->data_bit_comboBox->setEnabled(true);
    ui->parity_bit_comboBox->setEnabled(true);
    ui->stop_bit_comboBox->setEnabled(true);
    ui->action_open_port->setEnabled(true);
    ui->action_close_port->setEnabled(false);
    ui->action_send_data->setEnabled(false);

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
    ui->port_receive_textBrowser->insertPlainText("Clear Data function is being developed...");

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
