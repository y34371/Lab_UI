#ifndef XYPLOT_H
#define XYPLOT_H

#include <QWidget>
#include "qcustomplot.h"

class XyPlot : public QCustomPlot
{
    Q_OBJECT
public:

    struct DataPoint {
        qreal x;
        qreal y;
    };

    explicit XyPlot(QWidget *parent = nullptr);
    ~XyPlot();

    void receiveData(qreal x, qreal y);
    void updatePlot();

signals:

public slots:


private:
    int x_axis_range;
    QVector<DataPoint> rxd_buffer;
    int rxd_buffer_limit;
    int plot_buffer_limit;

};

#endif // XYPLOT_H
