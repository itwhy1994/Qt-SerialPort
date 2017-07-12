#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <string>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSerialPort *serial = new QSerialPort;

public slots:
    void on_clearButton_clicked(); // clear
    void on_sendButton_clicked(); // send data
    void on_openButton_clicked(); // open serialport
    void Read_Data(); // read data

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
