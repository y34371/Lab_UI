#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cmdlist.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionSetting->setEnabled(true);

    serial = new SerialPort(this);
    connect(serial,SIGNAL(PacketReceived()),this,SLOT(getDataFromPacket()));

    plot_update_timer = new QTimer(this);
    connect(plot_update_timer,SIGNAL(timeout()),this,SLOT(updateAllPlot()));
    plot_update_timer->start(200);

    tik_count = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete serial;
}


void MainWindow::getDataFromPacket()
{

    qint16 payload_int16[8];
    for(quint16 i=0;i<8;i++)
        serial->getInt16(payload_int16+i,i);


    float payload_float[4];
    for(quint16 i=0;i<4;i++)
        serial->getFloat(payload_float+i,i);

    ui->xyPlot_Ch1->receiveData(qreal(tik_count),qreal(payload_float[0]));
    ui->xyPlot_Ch2->receiveData(qreal(tik_count),qreal(payload_float[1]));
    ui->xyPlot_Ch3->receiveData(qreal(tik_count),qreal(payload_float[2]));
    ui->xyPlot_Ch4->receiveData(qreal(tik_count),qreal(payload_float[3]));
    tik_count++;

    qDebug() << payload_float[0] << payload_float[1] << payload_float[2] << payload_float[3];
}

void MainWindow::on_actionConnect_triggered()
{
    serial->openSerialPort();
    ui->actionConnect->setEnabled(false);
    ui->actionDisconnect->setEnabled(true);
    ui->actionSetting->setEnabled(false);
}

void MainWindow::on_actionDisconnect_triggered()
{
    serial->closeSerialPort();
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionSetting->setEnabled(true);
}

void MainWindow::on_actionSetting_triggered()
{
    serial->settingSerialPort();
}

void MainWindow::updateAllPlot()
{
    ui->xyPlot_Ch1->updatePlot();
    ui->xyPlot_Ch2->updatePlot();
    ui->xyPlot_Ch3->updatePlot();
    ui->xyPlot_Ch4->updatePlot();
}

void MainWindow::on_pushButton_clicked()
{
    tik_count++;
    ui->lcdNumber->display(tik_count);
}

void MainWindow::on_pushButton_2_clicked()
{
    tik_count--;
    ui->lcdNumber->display(tik_count);
}

void MainWindow::on_pushButton_3_clicked()
{
    serial->sendCmd(PWM_DIS,0,0,0);
}
