//
// Created by nrx on 14.01.2021.
//

#ifndef TESTSIMULINKMODEL_QCOMPORT_H
#define TESTSIMULINKMODEL_QCOMPORT_H

#include "sdd_protocol/connect/QIConnection.h"
#include <QtCore>

namespace sdd::conn {
    class QComPort : QIConnection {
        Q_OBJECT
    public slots:
        void setLightMode(Light light) override;
        void setTask(TaskPosition position) override;
        void setMode(Mode mode) override;
        void setState(State state) override;
    signals:
        void newState(State state) override;
    };
}

#endif //TESTSIMULINKMODEL_QCOMPORT_H
