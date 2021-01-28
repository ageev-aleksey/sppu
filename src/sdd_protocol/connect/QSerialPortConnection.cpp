//
// Created by nrx on 14.01.2021.
//

#include <sdd_protocol/PackageError.h>
#include "sdd_protocol/connect/QSerialPortConnection.h"
#include "sdd_protocol/connect/PackageBuffer.h"
#include "sdd_protocol/StatePackage.h"
#include "sdd_protocol/LightPackage.h"
#include "sdd_protocol/ModePackage.h"
#include "sdd_protocol/PositionPackage.h"
#include "sdd_protocol/PwmPackage.h"


#include <sstream>


void sdd::conn::QSerialPortConnection::send(Package &package)  {
    m_serialPort->write(&package.toBinary()[0]);
}

sdd::conn::QSerialPortConnection::QSerialPortConnection()
    : m_buffer(StatePackage::NUM_BYTES*2)
{
   m_serialPort = nullptr;
}

sdd::conn::State statePackageToStruct(sdd::StatePackage &pack) {
    sdd::conn::State state{};
    state.ox = pack.OX();
    state.oy = pack.OY();
    state.pwm.ox = pack.PWMX();
    state.pwm.oy = pack.PWMY();
    state.task.ox = pack.positionX();
    state.task.oy = pack.positionY();
    state.time = std::chrono::steady_clock::now();
    return state;
}

void sdd::conn::QSerialPortConnection::readIsReady() {
    StatePackage pack;
    std::unique_lock lock(m_mutex);
    auto readBufferSize = m_serialPort->bytesAvailable();
    if (readBufferSize >= pack.size()) {
        try {
            if (read(pack)) {
                lock.unlock();
                State state = statePackageToStruct(pack);
                allCall(state);
            }
        } catch (PackageParseError &exp) {
            qWarning() << "Invalid package: " << exp.what();
        } catch (std::exception &exp) {
            qWarning() << "Undefined error: " << exp.what();
        } catch (...) {
            qWarning() << "Undefined error";
        }
    }
}

// TODO(ageev) не работает
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
    packageWrite(&pack);
}

void sdd::conn::QSerialPortConnection::sendMode(sdd::conn::Mode package) {
    ModePackage pack;


    if (package.isOn) {
        pack.coilOn();
    } else {
        pack.coilOff();
    }
    packageWrite(&pack);
}

void sdd::conn::QSerialPortConnection::sendTaskPosition(sdd::conn::TaskPosition task) {
    if (m_serialPort == nullptr) {
        throw std::runtime_error("Serial port not inserted");
    }
    if (!m_serialPort->isOpen()) {
        throw std::runtime_error("Serial port not opened");
    }
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
    PositionPackage pack;

    pack.setPosX(static_cast<short>(task.ox));
    pack.setPosY(static_cast<short>(task.oy));
    packageWrite(&pack);
}

void sdd::conn::QSerialPortConnection::sendPwm(sdd::conn::Pwm pwm) {
    if (m_serialPort == nullptr) {
        throw std::runtime_error("Serial port not inserted");
    }
    if (!m_serialPort->isOpen()) {
        throw std::runtime_error("Serial port not opened");
    }
    PwmPackage pack;
    pack.pwmOx(static_cast<short>(pwm.ox));
    pack.pwmOy(static_cast<short>(pwm.oy));
    packageWrite(&pack);
}


bool sdd::conn::QSerialPortConnection::read(sdd::StatePackage &package) {
    QByteArray array = m_serialPort->read(package.size());
    m_buffer.addBytes(array.begin(), array.end());
    return m_buffer.formPackage(package);

//    std::vector<char> data;
//    data.reserve(array.size());
//    // TODO(ageev) избавиться от копирования
//    for (auto &el : array) {
//        data.push_back(el);
//    }
//    package.fromBinary(data);
}

void sdd::conn::QSerialPortConnection::setPort(std::shared_ptr<QSerialPort> port) {
    if (port == nullptr) {
        throw std::runtime_error("invalid serial port pointer");
    }
    if (!port->isOpen()) {
        throw std::runtime_error("Serial port not opened");
    }
    if (m_serialPort != nullptr) {
        QObject::disconnect(m_qtEventConnection);
    }

    m_serialPort = std::move(port);
    m_qtEventConnection = QObject::connect(m_serialPort.get(), &QSerialPort::readyRead, this, &QSerialPortConnection::readIsReady);
}

void sdd::conn::QSerialPortConnection::packageWrite(Package *pack) {
    auto bin = pack->toBinary();
    std::lock_guard lock(m_mutex);
    m_serialPort->write(bin.data(), bin.size());
}





