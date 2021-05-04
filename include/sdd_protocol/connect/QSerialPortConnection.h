//
// Created by nrx on 14.01.2021.
//

#ifndef TESTSIMULINKMODEL_QCOMPORT_H
#define TESTSIMULINKMODEL_QCOMPORT_H

#include "sdd_protocol/connect/QIConnection.h"
#include "sdd_protocol/connect/PackageBuffer.h"
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
    class QSerialPortConnection : public QIConnection {
        Q_OBJECT
    public:
        explicit QSerialPortConnection();
        void setPort(std::shared_ptr<QSerialPort> port);
        State recvState() ;
        void sendLight(Light package) override;
        void sendMode(Mode package) override;
        void sendTaskPosition(TaskPosition task) override;
        void sendPwm(Pwm pwm) override;
    public slots:
        void send(Package &package);

    private slots:
        void readIsReady();

    private:
        void packageWrite(Package *pack);
        bool read(StatePackage &package, size_t readSize);
        bool callbackIsContain(const std::function<Handle> &handler);
        State statePackageToStruct(sdd::StatePackage &pack);

        int m_currentIndex;
        std::shared_ptr<QSerialPort> m_serialPort;
        QMetaObject::Connection m_qtEventConnection;
        std::vector<std::function<Handle>> m_callBacks;
        std::mutex m_mutex;
        std::condition_variable m_cv;
        PackageBuffer m_buffer;
        QFile m_file;
    };
}

#endif //TESTSIMULINKMODEL_QCOMPORT_H
