//
// Created by nrx on 14.01.2021.
//

#ifndef TESTSIMULINKMODEL_QICONNECTION_H
#define TESTSIMULINKMODEL_QICONNECTION_H

#include "sdd_protocol/connect/States.h"
#include <QtCore>

namespace sdd::conn {
    class QIConnection : public QObject {
        Q_OBJECT
    public:
    public slots:
        virtual void setLightMode(Light light) = 0;
        virtual void setTask(TaskPosition position) = 0;
        virtual void setMode(Mode mode) = 0;
        virtual void setState(State state) = 0;
    signals:
        virtual void newState(State state) = 0;

    };
}


#endif //TESTSIMULINKMODEL_QICONNECTION_H
