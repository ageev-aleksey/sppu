//
// Created by nrx on 17.01.2021.
//

#ifndef SDDCLIENT_QISDDSTATEWIDGETFACTORY_H
#define SDDCLIENT_QISDDSTATEWIDGETFACTORY_H

#include "gui/QSddModelControl.h"

class QISddWidgetFactory {
public:
    virtual QISddStateWidget *makeWidget(QSettings &settings) = 0;
};

class QSddModelControlFactory : public QISddWidgetFactory {
public:
    QISddStateWidget *makeWidget(QSettings &settings) override;
};

class QSddSerialPortControlFactory : public QISddWidgetFactory {
public:
    QISddStateWidget *makeWidget(QSettings &settings) override;
};

#endif //SDDCLIENT_QISDDSTATEWIDGETFACTORY_H
