//
// Created by nrx on 07.08.2020.
//

#ifndef TESTSIMULINKMODEL_QCONDITIONALCONTROL_H
#define TESTSIMULINKMODEL_QCONDITIONALCONTROL_H
#include <QWidget>
#include <QCheckBox>
#include <QHBoxLayout>

class QConditionalControl : public QWidget {

public:

    QConditionalControl();
    void addLayout(QLayout *layout, size_t stretch = 0);
    void addWidget(QWidget *widget, size_t stretch = 0);
    bool isChecked();
    void setDisableChangingState(bool disableChangingState);
    bool isDisableChangingState();
private slots:
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
