//
// Created by nrx on 14.01.2021.
//

#ifndef TESTSIMULINKMODEL_QCOMPORT_H
#define TESTSIMULINKMODEL_QCOMPORT_H

#include "sdd_protocol/connect/IConnection.h"
#include "sdd_protocol/Package.h"
#include <QtCore>
#include <QtSerialPort>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <sdd_protocol/StatePackage.h>

// TODO(ageev) Выделить все пакате протокола в отдельный интерфейс, что бы можно было сделать фабрику для
//  разных видов протокола
namespace sdd::conn {
    class QSerialPortConnection : public QObject, public IConnection {
        Q_OBJECT
    public:
        explicit QSerialPortConnection(std::shared_ptr<QSerialPort> serialPort);
        QSerialPortConnection(std::shared_ptr<QSerialPort> serialPort, std::function<Handle> handler);
        State recvState() override;
        void sendLight(Light package) override;
        void sendMode(Mode package) override;
        void sendTaskPosition(TaskPosition task) override;
        void regCallbackDataReady(std::function<Handle> handler) override;
    public slots:
        void send(Package &package);

    private slots:
        void readIsReady();

    private:
        void read(StatePackage &package);

        std::shared_ptr<QSerialPort> m_serialPort;
        std::function<Handle> m_dataReceived;
        std::mutex m_mutex;
        std::condition_variable m_cv;
    };
}

#endif //TESTSIMULINKMODEL_QCOMPORT_H
