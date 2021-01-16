//
// Created by nrx on 14.01.2021.
//

#include <sdd_protocol/PackageError.h>
#include "sdd_protocol/connect/QSerialPortConnection.h"
#include "sdd_protocol/StatePackage.h"
#include "sdd_protocol/LightPackage.h"
#include "sdd_protocol/ModePackage.h"
#include "sdd_protocol/PositionPackage.h"

#include <sstream>


void sdd::conn::QSerialPortConnection::send(Package &package)  {
    m_serialPort->write(&package.toBinary()[0]);
}

sdd::conn::QSerialPortConnection::QSerialPortConnection(std::shared_ptr<QSerialPort> serialPort,
                                                        std::function<Handle> handler) {
    if (!serialPort->isOpen()) {
        throw std::runtime_error("Serial port not opened");
    }
    m_serialPort = std::move(serialPort);
    m_dataReceived = std::move(handler);
    QObject::connect(m_serialPort.get(), &QSerialPort::readyRead, this, &QSerialPortConnection::readIsReady);
}

sdd::conn::State statePackageToStruct(sdd::StatePackage &pack) {
    sdd::conn::State state{};
    state.ox = pack.OX();
    state.oy = pack.OY();
    state.pwmX = pack.PWMX();
    state.pwmY = pack.PWMY();
    state.task.ox = pack.positionX();
    state.task.oy = pack.positionY();
    state.time = std::chrono::steady_clock::now();
    return state;
}

void sdd::conn::QSerialPortConnection::readIsReady() {
    StatePackage pack;
    std::lock_guard lock(m_mutex);

    if (m_serialPort->readBufferSize() >= pack.size()) {
        try {
            read(pack);
            State state = statePackageToStruct(pack);
            m_dataReceived(state);
        } catch (PackageParseError &exp) {
            qWarning() << "Invalid package: " << exp.what();
        } catch (std::exception &exp) {
            qWarning() << "Undefined error: " << exp.what();
        } catch (...) {
            qWarning() << "Undefined error";
        }
    }
}


void sdd::conn::QSerialPortConnection::regCallbackDataReady(std::function<Handle> handler) {
    if (!handler) {
        throw std::runtime_error("handler pointer not be a nullptr");
    }
    m_dataReceived = std::move(handler);
}

sdd::conn::QSerialPortConnection::QSerialPortConnection(std::shared_ptr<QSerialPort> serialPort) {
    if (!serialPort->isOpen()) {
        throw std::runtime_error("Serial port not opened");
    }
    m_serialPort = std::move(serialPort);
}

sdd::conn::State sdd::conn::QSerialPortConnection::recvState() {
    StatePackage pack;
    std::unique_lock lock(m_mutex);

    if (m_serialPort->readBufferSize() < pack.size()) {
        m_cv.wait(lock, [&]() {return m_serialPort->readBufferSize() >= pack.size();});
    }
    read(pack);
    return statePackageToStruct(pack);
}

void sdd::conn::QSerialPortConnection::sendLight(sdd::conn::Light package) {
    LightPackage pack;


    if (package.blinking == 0) {
        pack.lightOff();
    } else if (package.blinking < 256) {
        pack.blinking(package.blinking);
    } else {
        pack.lightOn();
    }
    auto bin = pack.toBinary();

    std::lock_guard lock(m_mutex);
    m_serialPort->write(&bin[0]);
}

void sdd::conn::QSerialPortConnection::sendMode(sdd::conn::Mode package) {
    ModePackage pack;


    if (package.isOn) {
        pack.coilOn();
    } else {
        pack.coilOff();
    }
    auto bin = pack.toBinary();

    std::lock_guard lock(m_mutex);
    m_serialPort->write(&bin[0]);
}

void sdd::conn::QSerialPortConnection::sendTaskPosition(sdd::conn::TaskPosition task) {
    if (task.ox > 255) {
        std::stringstream msg;
        msg << "Invalid TaskPosition.ox:  TaskPosition{ox: " << task.ox << "; oy: " << task.oy << "}";
        throw std::runtime_error(msg.str());
    }

    if (task.oy > 255) {
        std::stringstream msg;
        msg << "Invalid TaskPosition.oy:  TaskPosition{ox: " << task.ox << "; oy: " << task.oy << "}";
        throw std::runtime_error(msg.str());
    }
    StatePackage pack;

    pack.setPositionX(static_cast<short>(task.ox));
    pack.setPositionY(static_cast<short>(task.oy));
    auto bin = pack.toBinary();
    std::lock_guard lock(m_mutex);
    m_serialPort->write(&bin[0]);

}

void sdd::conn::QSerialPortConnection::read(sdd::StatePackage &package) {
    QByteArray buffer = m_serialPort->read(package.size());
    // TODO (ageev) убрать копирование из одного буфера в другой
    package.fromBinary(std::vector<char>(buffer.begin(), buffer.end()));
}
