//
// Created by nrx on 17.01.2021.
//

#ifndef SDDCLIENT_QSDDDEVICECONTROL_H
#define SDDCLIENT_QSDDDEVICECONTROL_H
#include "gui/QISddStateWidget.h"
#include "sdd_protocol/connect/QSerialPortConnection.h"
#include <QtWidgets>

#include <memory>


class QSddDeviceControl : public QISddStateWidget {
public:
    explicit QSddDeviceControl(std::unique_ptr<QSerialPort> SerialPort);
    std::vector<sdd::conn::State> getSddStates() override;
private:
    void guiInit();

    std::unique_ptr<sdd::conn::QSerialPortConnection> m_pSender;
    std::shared_ptr<QSerialPort> m_pSerialPort;

    QVBoxLayout *mLayout = new QVBoxLayout;

    // Вывод текущего состояние устройства
    QLabel *mXPosition = new QLabel;
    QLabel *mYPosition = new QLabel;
    QLabel *mXSpeed = new QLabel;
    QLabel *mYSpeed = new QLabel;

    // Вкладка управления устройством
    QWidget *mControlWidget = new QWidget; /// Корневой виджет вкладки управления
    // 1. Выдача задания на управление
    QSpinBox *mOxTask = new QSpinBox;
    QSpinBox *mOyTask = new QSpinBox;
    // 2. Управление светодиодом
    QDoubleSpinBox *mLightBlinking = new QDoubleSpinBox;
    // 3. Установка скважности ШИМ
    QSpinBox *mPwmOx = new QSpinBox;
    QSpinBox *mPwmOy = new QSpinBox;
    QCheckBox *mModePwmControl = new QCheckBox;
    // Вкладка настройки КомПорта
    // 1. Скорость передачи битэ
    // 2. ...

};

#endif //SDDCLIENT_QSDDDEVICECONTROL_H
