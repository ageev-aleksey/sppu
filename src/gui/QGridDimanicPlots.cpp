#include "gui/QGridDimanicPlots.h"


QGridDimanicPlots::QGridDimanicPlots(QWidget *parent) : QWidget(parent) {
    setLayout(new QGridLayout);
}

QGridDimanicPlots::~QGridDimanicPlots() noexcept = default;

QCustomPlot* QGridDimanicPlots::addPlot(size_t row, size_t col, const Property *property) {
    const Property *p = property;
    if (p == nullptr) {
        p = &mProp;
        size_t n = p->limits.size();
        if (!(p->penWidth.size() == n && p->color.size() == n && p->lineStyle.size() == n)) {
            throw std::runtime_error("invalid size property elements");
        }
    }
    if (mPlots.size() < row + 1) {
        mPlots.resize(row + 1);
    }
    if (mPlots[row].size() < col + 1) {
        mPlots[row].resize(col + 1);
    }
    auto &pInfo = mPlots[row][col];
    if (pInfo.plot == nullptr) {
        pInfo.plot = new QCustomPlot;
        QGridLayout *l = dynamic_cast<QGridLayout *>(layout());
        l->addWidget(pInfo.plot, row, col);

        size_t num_graphs = p->color.size();
        for (int i = 0; i < num_graphs; i++) {
            pInfo.plot->addGraph();
            pInfo.plot->graph(i)->setLineStyle(p->lineStyle[i]);
            pInfo.plot->graph(i)->setPen(QPen(p->color[i], mProp.penWidth[i]));
            pInfo.plot->xAxis->setRange(p->limits[i].key.first, p->limits[i].key.second);
            pInfo.plot->xAxis->setRange(p->limits[i].value.first, p->limits[i].value.second);
            pInfo.limits = p->limits[i];
            return pInfo.plot;
        }
        throw std::runtime_error("Plot exists");
    }
}

QCustomPlot* QGridDimanicPlots::plot(size_t row, size_t col) {
    if (mPlots.size() > row && mPlots[row].size() > col) {
        return mPlots[row][col].plot;
    } else {
        throw std::range_error("plot not found");
    }
}

void QGridDimanicPlots::addPlotData(size_t row, size_t col, size_t numberGraph, double key, double value) {
    if ( !(mPlots.size() > row && mPlots[row].size() > col && mPlots[row][col].plot != nullptr) ) {
        throw std::runtime_error("plot not exist");
    }
    auto &info = mPlots[row][col];
    info.plot->graph(numberGraph)->addData(key, value);
    if (mPlots[row][col].limits.valueType == Property::Limits::Type::AUTO) {
        bool isFoundRange = true;
        double minValue = info.plot->graph(numberGraph)->getValueRange(isFoundRange).lower + info.limits.value.first;
        double maxValue = info.plot->graph(numberGraph)->getValueRange(isFoundRange).upper + info.limits.value.second;
        info.plot->yAxis->setRange(minValue, maxValue);
    }
    if (mPlots[row][col].limits.keyType == Property::Limits::Type::AUTO) {
        bool isFoundRange = true;
        double minKey = info.plot->graph(numberGraph)->getKeyRange(isFoundRange).lower + info.limits.key.first;
        double maxKey = info.plot->graph(numberGraph)->getKeyRange(isFoundRange).upper + info.limits.key.second;
        info.plot->xAxis->setRange(minKey, maxKey);
    } else if (mPlots[row][col].limits.keyType == Property::Limits::Type::WINDOW) {
        bool isFoundRange = true;
        double minKey = info.plot->graph(numberGraph)->getKeyRange(isFoundRange).lower;
        double maxKey = info.plot->graph(numberGraph)->getKeyRange(isFoundRange).upper;
        if (maxKey - minKey > info.limits.key.first) {
            // info.plot->graph(numberGraph)->data()->removeBefore(maxKey-mPlots[row][col].limits.key.first);
        }
       info.plot->xAxis->setRange(maxKey-mPlots[row][col].limits.key.first, maxKey);
    }
    // mPlots[row][col].plot->replot(QCustomPlot::RefreshPriority::rpQueuedReplot);
}

void QGridDimanicPlots::allDataClear() {
    for (auto &el : mPlots) {
        for (auto p : el) {
            if (p.plot != nullptr) {
                for (int i = 0; i < p.plot->graphCount(); i++) {
                    p.plot->graph(i)->data()->clear();
                }
            }
        }
    }
}

void QGridDimanicPlots::allReplot() {
    for (auto &el : mPlots) {
        for (auto p : el) {
            if (p.plot != nullptr) {
                for (int i = 0; i < p.plot->graphCount(); i++) {
                    p.plot->replot();
                }
            }
        }
    }
}


