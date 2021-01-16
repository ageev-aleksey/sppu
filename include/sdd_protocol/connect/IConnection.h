//
// Created by nrx on 14.01.2021.
//

#ifndef TESTSIMULINKMODEL_QICONNECTION_H
#define TESTSIMULINKMODEL_QICONNECTION_H

#include "sdd_protocol/connect/States.h"
#include "sdd_protocol/Package.h"
#include <QtCore>
#include <functional>

namespace sdd::conn {
    class IConnection {
    public:
        using Handle = void(State &state);

        virtual State recvState() = 0;
        virtual void sendLight(Light package) = 0;
        virtual void sendMode(Mode package) = 0;
        virtual void sendTaskPosition(TaskPosition task) = 0;
        virtual void regCallbackDataReady(std::function<Handle> handler) = 0;
    };

} // namespace
#endif //TESTSIMULINKMODEL_QICONNECTION_H
