//
// Created by nrx on 07.08.2020.
//

#ifndef TESTSIMULINKMODEL_QCONDITIONALCONTROL_H
#define TESTSIMULINKMODEL_QCONDITIONALCONTROL_H
#include <QWidget>
#include <QCheckBox>
#include <QHBoxLayout>


/**
 * Виджет обертка, добавлющий к обарчиваемому виджету
 * checkbox активности оборачиваемого виджета.
 * Если checkbox, то и обарачивемый виджет активен.
 */
class QConditionalControl : public QWidget {

public:

    QConditionalControl();
    /**
     * Добавить компоновщик
     * @param layout  компоновка
     * @param stretch
     */
    void addLayout(QLayout *layout, size_t stretch = 0);
    /**
     * Добавить виджет
     * @param widget виджет
     * @param stretch
     */
    void addWidget(QWidget *widget, size_t stretch = 0);
    /**
     * Проверить активность виджета
     * @return true - виджет активен (галочка установлена)
     */
    bool isChecked();
    /**
     * Установка режима работы
     * @param disableChangingState true - перевести в отключенный режим (сянть галочку)
     * false - перевести в активный режим (установить галочку)
     */
    void setDisableChangingState(bool disableChangingState);
    /// Проверка на отключенность виджета
    bool isDisableChangingState();
private slots:
    /// Слот для обновления состояния виджетов
    void stateUpdate(int state);
private:
    struct Controller  {
        explicit Controller(QWidget *widget);
        void setEnabled(bool enabled);
        bool mIsEnabled;
        bool mPrivateChange;
        QWidget *mWidget;
    };

    Controller* findController(QWidget *widget);
    QWidgetList getWidgets(QLayout *layout);


    bool eventFilter(QObject *object, QEvent *event) override;
    void setEnabledWidgetsInLayout(QLayout *layout, bool enable);
    void setEventFilterForWidgets(QLayout *layout);
    QCheckBox *mCheckBox = new QCheckBox("Enable");
    QVBoxLayout *mLayout = new QVBoxLayout;
    QList<Controller> mExternWidgets;

};
#endif //TESTSIMULINKMODEL_QCONDITIONALCONTROL_H
