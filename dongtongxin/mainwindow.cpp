#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    foreach( const QSerialPortInfo &Info,QSerialPortInfo::availablePorts())//读取串口信息
        {
            qDebug() << "portName    :"  << Info.portName();//调试时可以看的串口信息
            qDebug() << "Description   :" << Info.description();
            qDebug() << "Manufacturer:" << Info.manufacturer();

            QSerialPort serial;
            serial.setPort(Info);

            if( serial.open( QIODevice::ReadWrite) )//如果串口是可以读写方式打开的
            {
                ui->comComboBox->addItem( Info.portName() );//在comboBox那添加串口号
                serial.close();//然后自动关闭等待人为开启（通过那个打开串口的PushButton）
            }
        }
}
/* on_comButton_clicked
 * 描述：点击打开串口
 * 输入：无
 * 输出：无
 */
void MainWindow::on_pushButton_comopen_clicked()
{
    if(comStat == 0)
    {
      int baudId = ui->baudRateComboBox->currentIndex();
     QSerialPort::BaudRate baudRate;
    switch (baudId) {
    case 0:
        baudRate = QSerialPort::Baud1200;
        break;
    case 1:
        baudRate = QSerialPort::Baud2400;
        break;
    case 2:
        baudRate = QSerialPort::Baud4800;
        break;
    case 3:
        baudRate = QSerialPort::Baud9600;
        break;
    case 4:
        baudRate = QSerialPort::Baud19200;
        break;
    case 5:
        baudRate = QSerialPort::Baud38400;
        break;
    case 6:
        baudRate = QSerialPort::Baud57600;
        break;
    case 7:
        baudRate = QSerialPort::Baud115200;
        break;
    default:
        QMessageBox::warning(NULL,"警告","无效波特率！",QMessageBox::Yes);
        break;
    }

    //获取数据位
    int dataBitsId = ui->dataBitsComboBox->currentIndex();
    QSerialPort::DataBits dataBits;
    switch (dataBitsId) {
    case 0:
        dataBits = QSerialPort::Data5;
        break;
    case 1:
        dataBits = QSerialPort::Data6;
        break;
    case 2:
        dataBits = QSerialPort::Data7;
        break;
    case 3:
        dataBits = QSerialPort::Data8;
        break;
    default:
        QMessageBox::warning(NULL,"警告","无效数据位！",QMessageBox::Yes);
        break;
    }

    //获取校验位
    int parityId = ui->parityComboBox->currentIndex();
    QSerialPort::Parity parity;
    switch (parityId) {
    case 0:
        parity = QSerialPort::NoParity;
        break;
    case 1:
        parity = QSerialPort::EvenParity;
        break;
    case 2:
        parity = QSerialPort::OddParity;
        break;
    case 3:
        parity = QSerialPort::SpaceParity;
        break;
    case 4:
        parity = QSerialPort::MarkParity;
        break;
    default:
        QMessageBox::warning(NULL,"警告","无效校验位！",QMessageBox::Yes);
        break;
    }

    //获取停止位
    int stopBitsId = ui->stopBitsComboBox->currentIndex();
    QSerialPort::StopBits stopBits;
    switch (stopBitsId) {
    case 0:
        stopBits = QSerialPort::OneStop;
        break;
    case 1:
        stopBits = QSerialPort::OneAndHalfStop;
        break;
    case 2:
        stopBits = QSerialPort::TwoStop;
        break;
    default:
        QMessageBox::warning(NULL,"警告","无效停止位！",QMessageBox::Yes);
        break;
    }

    QString comName = ui->comComboBox->currentText();

    //打开串口
    comPort = new QSerialPort(comName);
    if(comPort->open(QIODevice::ReadWrite))
    {    qDebug() << ui->comComboBox->currentText();
        comPort->setBaudRate(baudRate);
        comPort->setStopBits(stopBits);
        comPort->setParity(parity);
        comPort->setDataBits(dataBits);
        comPort->setFlowControl(QSerialPort::NoFlowControl);        //没有流控

        comPort->clearError();
        comPort->clear();
        connect(comPort,SIGNAL(readyRead()),this,SLOT(readMyCom()));
        //修改UI
        ui->pushButton_comopen->setText("关闭串口");
        ui->baudRateComboBox->setEnabled(false);
        ui->dataBitsComboBox->setEnabled(false);
        ui->parityComboBox->setEnabled(false);
        ui->stopBitsComboBox->setEnabled(false);
        ui->comComboBox->setEnabled(false);
        ui->comRefreshButton->setEnabled(false);
        comStat = 1;
    }
    else
    {
        //打开串口失败
        QMessageBox::warning(NULL,"警告","打开串口失败",QMessageBox::Yes);
        delete comPort;
    }
    }

    else
    {
        //关闭串口
        comPort->close();
        delete comPort;
        //ui调整
        ui->pushButton_comopen->setText("打开串口");
        ui->baudRateComboBox->setEnabled(true);
        ui->dataBitsComboBox->setEnabled(true);
        ui->parityComboBox->setEnabled(true);
        ui->stopBitsComboBox->setEnabled(true);
        ui->comComboBox->setEnabled(true);
        ui->comRefreshButton->setEnabled(true);
        comStat = 0;
    }


}
/* on_comRefreshButton_clicked
 * 描述：刷新按钮按下，刷新com口信息
 * 输入：无
 * 输出：无
 */

void MainWindow::on_comRefreshButton_clicked()
{
    //清空原来内容
    ui->comComboBox->clear();
    //遍历可用串口号
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->comComboBox->addItem(info.portName());
    }
}

void MainWindow::on_pushButton_send_clicked()
{
    comPort->write( ui->lineEdit_write->text().toUtf8() );
}
void MainWindow::readMyCom()
{
    requestData = comPort->readAll();//用requestData存储从串口那读取的数据
    if(requestData!=NULL)//如果读取到有数据，就显示出来。
    {
        ui->textEdit_read->append(requestData);
    }
    requestData.clear();    //清除缓冲区
}

void MainWindow::on_pushButton_clear_clicked()
{
    ui->textEdit_read->clear();
    ui->lineEdit_write->clear();
}




