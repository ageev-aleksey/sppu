//
// Created by nrx on 01.08.2020.
//

#ifndef TESTSIMULINKMODEL_QGRIDDIMANICPLOTS_H
#define TESTSIMULINKMODEL_QGRIDDIMANICPLOTS_H
#include <QWidget>
#include <qcustomplot.h>
#include <utility>

class QGridDynamicPlots : public QWidget {
    Q_OBJECT
public:

    struct Property {
        struct  Limits {
            enum class Type {
                FIXED, AUTO, WINDOW
            };
            std::pair<int, int> key = {-10, 10};
            std::pair<int, int> value = {-10, 10};
            Type keyType = Type::FIXED;
            Type valueType = Type::FIXED;
        };

        std::vector<Qt::GlobalColor> color = {Qt::GlobalColor::blue};
        std::vector<size_t> penWidth = {3};
        std::vector<QCPGraph::LineStyle> lineStyle = {QCPGraph::LineStyle::lsLine};
        std::vector<Limits> limits = {{}};
    };

    explicit QGridDynamicPlots(QWidget *parnet = nullptr);
    // explicit QGridDynamicPlots(const Property &prop, QWidget *parnet = nullptr);
    ~QGridDynamicPlots() override;
    QCustomPlot* addPlot(size_t row, size_t col, const Property *p = nullptr);
    QCustomPlot* plot(size_t row, size_t col);
    void addPlotData(size_t row, size_t col, size_t numberGraph, double key, double value);
    void allDataClear();
    void allReplot();
    //void setDefaultProperty(const Property &p);
    //void setPlotProperty(size_t row, size_t col, const Property &p);
private:
    struct Element {
        QCustomPlot *plot = nullptr;
        Property::Limits limits;
    };
    std::vector<std::vector<Element>> mPlots;
    Property mProp = {};
};

#endif //TESTSIMULINKMODEL_QGRIDDIMANICPLOTS_H
