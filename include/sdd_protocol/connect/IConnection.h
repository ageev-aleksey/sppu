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
        using Handle = void(const State &state);

        virtual void sendLight(Light package) = 0;
        virtual void sendMode(Mode package) = 0;
        virtual void sendTaskPosition(TaskPosition task) = 0;
        virtual void addCallbackDataReady(const std::function<Handle> &handler);
       // virtual void removeCallback(const std::function<Handle> &handler);
    protected:
        void allCall(const State &state);
    private:
       // bool callbackIsContain(const std::function<Handle> &handler);
        std::mutex m_callbackMutex;
        std::vector<std::function<Handle>> m_callbacks;
    };

} // namespace
#endif //TESTSIMULINKMODEL_QICONNECTION_H
