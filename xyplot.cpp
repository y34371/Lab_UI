#include "xyplot.h"

XyPlot::XyPlot(QWidget *parent) : QCustomPlot(parent)
{
    rxd_buffer_limit = 20000;
    plot_buffer_limit = 4000;
    x_axis_range = 1;
    this->addGraph();
}

XyPlot::~XyPlot()
{

}

void XyPlot::receiveData(qreal x, qreal y)
{
    DataPoint temp;
    temp.x = x;
    temp.y = y;
    rxd_buffer.append(temp);
//    qDebug() << x << y;
//    qDebug() << rxd_buffer.size();

    if(rxd_buffer.size() > rxd_buffer_limit)
        rxd_buffer.removeFirst();
}

void XyPlot::updatePlot()
{
    if(rxd_buffer.isEmpty())
        return;
    QVector<QCPGraphData> plot_buffer;
    QCPGraphData temp;
    int rxd_index = rxd_buffer.size()-1;
    int plot_index = 0;
    qreal max_value = rxd_buffer.at(rxd_index).y;
    qreal min_value = rxd_buffer.at(rxd_index).y;
    while(rxd_index >= 0 && plot_index < plot_buffer_limit)
    {
        temp.key = rxd_buffer.at(rxd_index).x;
        temp.value = rxd_buffer.at(rxd_index).y;

        if(temp.value > max_value)
            max_value = temp.value;
        if(temp.value < min_value)
            min_value = temp.value;

        plot_buffer.append(temp);
        rxd_index -= x_axis_range;
        plot_index ++;
    }
//    qDebug() << rxd_buffer.size() << plot_buffer.size();

    this->graph(0)->data()->set(plot_buffer);
    this->xAxis->setRange(plot_buffer.first().key,plot_buffer.last().key);
//    qDebug() << min_value << max_value;
    qreal y_lim_low,y_lim_high,y_margin;
    y_margin = (max_value - min_value) * 0.1;
    y_lim_low = min_value - y_margin;
    y_lim_high = max_value + y_margin;
    this->yAxis->setRange(y_lim_low,y_lim_high);
//    this->yAxis->setRange(59.9,60.1);
    this->replot();

//    qreal x = QCPAxisTickerDateTime::dateTimeToKey(t);
//    data_1.append(QCPGraphData(x,y));
//    if(data_1.size() > buffer_size_limit)
//        data_1.removeFirst();
//    qDebug() << x;
//    qDebug() << data_1.size() << data_1.last().key << data_1.last().value << QTime::currentTime().toString("hh.mm.ss");
//    this->graph(0)->data()->set(data_1);
//    this->xAxis->setRange(data_1.first().key,data_1.last().key);
////    this->xAxis->setRange(-60*2.5, 60*11);
//    this->replot();
}
