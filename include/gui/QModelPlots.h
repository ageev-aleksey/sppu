//
// Created by nrx on 31.07.2020.
//

#ifndef TESTSIMULINKMODEL_QMODELPLOTS_H
#define TESTSIMULINKMODEL_QMODELPLOTS_H
#include <qcustomplot.h>
#include <QWidget>


class QModelPlots : public QWidget {
    Q_OBJECT
public:
    struct Data {
        double XPosition;
        double ZPosition;
        double XSpeed;
        double ZSpeed;
        double time;
    };
    QModelPlots(QWidget *widget = nullptr);
    QModelPlots(size_t bufferSize);
    QModelPlots(QWidget *widget, size_t bufferSize);
    ~QModelPlots();
public slots:
    void setBufferSize(size_t size);
    void addNextPoint(const Data &data);
signals:
    void updateBufferSize(size_t bufferSize);
    void  allReplot();
private:
    void init(size_t buffer_size = 1000);

    size_t mBufferSize;
    QVector<double> mXDataXPosition;
    QVector<double> mXDataZPosition;
    QVector<double> mXDataXSpeed;
    QVector<double> mXDataZSpeed;
    QVector<double> mXPhasePortrait;
    QVector<double> mYPhasePortrait;
    QVector<double> mYDataTime;
    QCustomPlot *mXPositionPlot;
    QCustomPlot *mZPositionPlot;
    QCustomPlot *mXSpeedPlot;
    QCustomPlot *mZSpeedPlot;
    QCustomPlot mPositionPhasePortraitPlot;
};

Q_DECLARE_METATYPE(QModelPlots::Data);

#endif //TESTSIMULINKMODEL_QMODELPLOTS_H
