#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QtNetwork>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include<QTimer>
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
    void on_pushButton_comopen_clicked();
    void on_comRefreshButton_clicked();
    void on_pushButton_send_clicked();
    void readMyCom();//用于读取数据
    void on_pushButton_clear_clicked();




private:
    Ui::MainWindow *ui;
    QSerialPort *comPort;
    int comStat;
    QByteArray requestData;
    QTimer *timer;//（用于计时
};

#endif // MAINWINDOW_H
