#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //find usable serialport
    QSerialPortInfo serialinfo;
    QList<QSerialPortInfo> infos = serialinfo.availablePorts();

    //QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    if (infos.empty())
    {
        ui->PortBox->addItem("None");
        infos = serialinfo.availablePorts();
    }
    foreach(QSerialPortInfo info, infos)
    {
        ui->PortBox->addItem(info.portName());
    }

    //setup baudrate menu display
    ui->BaudBox->setCurrentIndex(3);

    //turn on sendButton
    ui->sendButton->setEnabled(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_clearButton_clicked()
{
    ui->textEdit->clear();
    ui->textEdit_2->clear();
}

void MainWindow::on_sendButton_clicked()
{
    serial->write(ui->textEdit_2->toPlainText().toLatin1());
    serial->write("\n");
}

void MainWindow::on_openButton_clicked()
{
    if (ui->openButton->text() == tr("打开串口"))
    {
        serial->setPortName(ui->PortBox->currentText()); // 设置串口名
        serial->open(QIODevice::ReadWrite); // 打开串口
        serial->setBaudRate(ui->BaudBox->currentText().toInt()); // 设置波特率
        switch(ui->BitNumBox->currentText().toInt()) // 设置数据位数
        {
        case 5:
            serial->setDataBits(QSerialPort::Data5);
            break;
        case 6:
            serial->setDataBits(QSerialPort::Data6);
            break;
        case 7:
            serial->setDataBits(QSerialPort::Data7);
            break;
        case 8:
            serial->setDataBits(QSerialPort::Data8);
            break;
        default:
            break;
        }
        switch(ui->ParityBox->currentText().toInt()) // 设置奇偶校验
        {
        case 0:
            serial->setParity(QSerialPort::NoParity);
            break;
        case 2:
            serial->setParity(QSerialPort::EvenParity);
            break;
        case 3:
            serial->setParity(QSerialPort::OddParity);
            break;
        case 4:
            serial->setParity(QSerialPort::SpaceParity);
            break;
        case 5:
            serial->setParity(QSerialPort::MarkParity);
            break;
        default:
            break;
        }
        switch(ui->StopBox->currentText().toInt()) // 设置停止位
        {
        case 1:
            serial->setStopBits(QSerialPort::OneStop);
            break;
        case 2:
            serial->setStopBits(QSerialPort::TwoStop);
            break;
        default:
            break;
        }
        serial->setFlowControl(QSerialPort::NoFlowControl); // 设置流控制
        // 关闭参数选择使能
        ui->PortBox->setEnabled(false);
        ui->BaudBox->setEnabled(false);
        ui->BitNumBox->setEnabled(false);
        ui->ParityBox->setEnabled(false);
        ui->StopBox->setEnabled(false);
        ui->openButton->setText(tr("关闭串口"));
        ui->sendButton->setEnabled(true);
        // 连接信号槽
        QObject::connect(serial, &QSerialPort::readyRead, this, &MainWindow::Read_Data);
    }
    else
    {
        // 关闭串口
        serial->clear();
        serial->close();
        serial->deleteLater();

        // 打开参数选择使能
        ui->PortBox->setEnabled(true);
        ui->BaudBox->setEnabled(true);
        ui->BitNumBox->setEnabled(true);
        ui->ParityBox->setEnabled(true);
        ui->StopBox->setEnabled(true);
        ui->openButton->setText(tr("打开串口"));
        ui->sendButton->setEnabled(false);
    }
}

void MainWindow::Read_Data()
{
    QByteArray buf = NULL;
    buf = serial->readAll();
    if (!buf.isEmpty())
    {
        QString str = tr(buf);
        ui->textEdit->append(str);
    }
    buf.clear();
}

