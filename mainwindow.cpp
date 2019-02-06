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

    ui->xyPlot_Ch1->receiveData(qreal(tik_count),qreal(payload_int16[0]));
    ui->xyPlot_Ch2->receiveData(qreal(tik_count),qreal(payload_int16[2]));
    ui->xyPlot_Ch3->receiveData(qreal(tik_count),qreal(payload_int16[3]));
    ui->xyPlot_Ch4->receiveData(qreal(tik_count),qreal(payload_int16[4]));
    tik_count++;

    ui->editFpgaStatus->setText(QString::number(payload_int16[0]));
    ui->editFaultValue->setText(QString::number(payload_int16[1]));
    ui->editChbState->setText(QString::number(payload_int16[2]));
    ui->editFaultPrepare->setText(QString::number(payload_int16[3]));

//    qDebug() << payload_int16[0] << payload_int16[1] << payload_int16[2] << payload_int16[3] << payload_int16[4];
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


void MainWindow::on_buttonMainClose_clicked()
{
    serial->sendCmd(RELAY_1_CLOSE,0,0,0);
}

void MainWindow::on_buttoMainOpen_clicked()
{
    serial->sendCmd(RELAY_1_OPEN,0,0,0);
}

void MainWindow::on_buttonSsrClose_clicked()
{
    serial->sendCmd(RELAY_2_CLOSE,0,0,0);
}

void MainWindow::on_buttonSsrOpen_clicked()
{
    serial->sendCmd(RELAY_2_OPEN,0,0,0);
}

void MainWindow::on_buttonLoadClose_clicked()
{
    serial->sendCmd(LOAD_CLOSE,0,0,0);
}

void MainWindow::on_buttonLoadOpen_clicked()
{
    serial->sendCmd(LOAD_OPEN,0,0,0);
}

void MainWindow::on_buttonFaultDetEn_clicked()
{
    serial->sendCmd(FAULT_DET_EN,0,0,0);
}

void MainWindow::on_buttonFaultDetDis_clicked()
{
    serial->sendCmd(FAULT_DET_DIS,0,0,0);
}

void MainWindow::on_buttonTrigger_clicked()
{
    serial->sendCmd(TRIGGER,0,0,0);
}

void MainWindow::on_buttonChbOn_clicked()
{
    serial->sendCmd(CHB_ON,0,0,0);
}

void MainWindow::on_buttonChbOff_clicked()
{
    serial->sendCmd(CHB_OFF,0,0,0);
}
