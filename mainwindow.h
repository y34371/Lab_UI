#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialport.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void getDataFromPacket();

    void on_actionConnect_triggered();

    void on_actionDisconnect_triggered();

    void on_actionSetting_triggered();

    void updateAllPlot();

    void on_buttonMainClose_clicked();

    void on_buttoMainOpen_clicked();

    void on_buttonSsrClose_clicked();

    void on_buttonSsrOpen_clicked();

private:
    Ui::MainWindow *ui;
    SerialPort *serial;
    QTimer *plot_update_timer;

    int tik_count;
};

#endif // MAINWINDOW_H
