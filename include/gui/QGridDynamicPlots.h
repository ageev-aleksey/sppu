//
// Created by nrx on 01.08.2020.
//

#ifndef TESTSIMULINKMODEL_QGRIDDIMANICPLOTS_H
#define TESTSIMULINKMODEL_QGRIDDIMANICPLOTS_H
#include <QWidget>
#include <qcustomplot.h>
#include <utility>

/**
 * Виджет сетки графиков
 */
class QGridDynamicPlots : public QWidget {
    Q_OBJECT
public:
    /// Свойства отображения графика
    struct Property {
        /// Отображаемая область значений
        struct  Limits {
            enum class Type {
                /// Фиксированная область значений
                FIXED,
                /// Атоматически расширяемя область значений
                AUTO,
                /// Скользящее окно. Окно фиксированного размера, которое движется
                /// за обноволяемыми данными
                WINDOW
            };
            /// Область значений по оси абцисс
            std::pair<int, int> key = {-10, 10};
            /// Область значений по оси ординат
            std::pair<int, int> value = {-10, 10};
            /// Тип ограниченйи по оси абцисс
            Type keyType = Type::FIXED;
            /// Тип ограничений по оси ординат
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
    /**
     * Создать новый график по указанным координатам сетки
     * @param row строка
     * @param col колонка
     * @param p свойства создаваемого графика
     * @return  указатель на созданный график, который был размещен по указанным координатам
     */
    QCustomPlot* addPlot(size_t row, size_t col, const Property *p = nullptr);
    /**
     * Получить указатель на график по указанным координатам
     * @param row строка сетки
     * @param col колонка сетки
     * @return график по заданным коорднатам. Если график отсутсвует то nullptr
     */
    QCustomPlot* plot(size_t row, size_t col);
    /**
     * Добавить точку к указанной кривой на графике
     * @param row  строка расположения
     * @param col  колонка расположения
     * @param numberGraph  номер кривой на графике
     * @param key  значение по оси абцисс
     * @param value  значение по оси ордината
     */
    void addPlotData(size_t row, size_t col, size_t numberGraph, double key, double value);
    /**
     * Очитсиит данные у всех графиков сетки
     */
    void allDataClear();
    /**
     * Обновить отображение всех графиков сетки
     */
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
