#include "gui/QModelPlots.h"
#include <utility>
#include <algorithm>
#include <assert.h>

QModelPlots::~QModelPlots() noexcept {}


void graph_init(QCustomPlot *plot, const std::pair<int, int> &xLimits, const std::pair<int, int> &yLimits,
        Qt::GlobalColor color, size_t penSize) {
    plot->addGraph();
    plot->graph(0)->setPen(QPen(color, penSize));
    plot->xAxis->setRange(xLimits.first, xLimits.second);
    plot->yAxis->setRange(yLimits.first, yLimits.second);
}

QModelPlots::QModelPlots(size_t bufferSize) :QWidget(nullptr) {
    init(bufferSize);
}

QModelPlots::QModelPlots(QWidget *widget) : QWidget(widget) {
    init();
}

QModelPlots::QModelPlots(QWidget *widget, size_t bufferSize) : QWidget(widget) {
    init(bufferSize);
}

void QModelPlots::setBufferSize(size_t size) {
    if (mBufferSize != size) {
        mBufferSize = size;
        emit updateBufferSize(mBufferSize);
    }
}


void QModelPlots::init(size_t bufferSize) {
    qRegisterMetaType<Data>("Data");
    setBufferSize(bufferSize);
    auto layout = new QGridLayout;
    mXPositionPlot = new QCustomPlot;
    mZPositionPlot = new QCustomPlot;
    mXSpeedPlot = new QCustomPlot;
    mZSpeedPlot = new QCustomPlot;
    setLayout(layout);
    graph_init(mXPositionPlot, {0, 10}, {-0.7, 0.7}, Qt::blue, 3);
    graph_init(mZPositionPlot, {0, 10}, {-0.7, 0.7}, Qt::blue, 3);
    graph_init(mXSpeedPlot, {0, 10}, {-10, 10}, Qt::blue, 3);
    graph_init(mZSpeedPlot, {0, 10}, {-10, 10}, Qt::blue, 3);
    layout->addWidget(mXPositionPlot, 0, 0);
    layout->addWidget(mZPositionPlot, 0, 1);
    layout->addWidget(mXSpeedPlot, 1, 0);
    layout->addWidget(mZSpeedPlot, 1, 1);
}

void QModelPlots::addNextPoint(const Data &data) {
//    if(mXDataXPosition.size() >= mBufferSize) {
//        mXDataXPosition.pop_front();
//        mXDataZPosition.pop_front();
//        mXDataXSpeed.pop_front();
//        mXDataZSpeed.pop_front();
//        mYDataTime.pop_front();
//
//
//    }
    mXDataXPosition.push_back(data.XPosition);
    mXDataZPosition.push_back(data.ZPosition);
    mXDataXSpeed.push_back(data.XSpeed);
    mXDataZSpeed.push_back(data.ZSpeed);
    mYDataTime.push_back(data.time);

    double timeBegin =  mYDataTime.at(0), timeEnd = data.time;
    auto min_value = std::min_element(mXDataXPosition.begin(), mXDataXPosition.end());
    auto max_value = std::max_element(mXDataXPosition.begin(), mXDataXPosition.end());
    mXPositionPlot->xAxis->setRange(0, timeEnd+1.0);
    mXPositionPlot->yAxis->setRange(*min_value-1.0, *max_value+1.0);
    mZPositionPlot->xAxis->setRange(0, timeEnd+1);
    mXSpeedPlot->xAxis->setRange(timeBegin, timeEnd);
    mZSpeedPlot->xAxis->setRange(timeBegin, timeEnd);
    assert(data.time >= 0);
    mXPositionPlot->graph(0)->addData(data.time, data.XPosition);
//    mZPositionPlot->graph(0)->setData(mXDataZPosition, mYDataTime);
//    mXSpeedPlot->graph(0)->setData(mXDataXSpeed, mYDataTime);
//    mZSpeedPlot->graph(0)->setData(mXDataZSpeed, mYDataTime);

    mXPositionPlot->replot(QCustomPlot::rpQueuedReplot);
//    mZPositionPlot->replot(QCustomPlot::rpQueuedReplot);
//    mXSpeedPlot->replot(QCustomPlot::rpQueuedReplot);
//    mZSpeedPlot->replot(QCustomPlot::rpQueuedReplot);

    emit allReplot();
}
