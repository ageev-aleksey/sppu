#include "gui/QSddSerialPortControl.h"
#include <iostream>


QSddSerialPortControl::QSddSerialPortControl(std::unique_ptr<QSerialPort> connection)
    : m_pSerialPort(std::move(connection)),  m_pSender()
{
    setLayout(mLayout);
    guiInit();
    controlSettings();
    serialInit();
}

std::vector<sdd::conn::State> QSddSerialPortControl::getSddStates() {
    std::lock_guard lock (m_mutexStates);
    return std::move(m_vStates);
}

void QSddSerialPortControl::guiInit() {
    auto *dashBoardLayout = new QHBoxLayout;

    auto textDeviceStateLayout = new QGridLayout;
    auto *box = new QGroupBox("Current device state");
    box->setLayout(textDeviceStateLayout);
    textDeviceStateLayout->addWidget(new QLabel("X position:"), 0, 0);
    textDeviceStateLayout->addWidget(new QLabel("Z position:"), 1, 0);
    textDeviceStateLayout->addWidget(new QLabel("X speed:"), 2, 0);
    textDeviceStateLayout->addWidget(new QLabel("Z speed:"), 3, 0);
    textDeviceStateLayout->addWidget(mXPosition, 0, 1);
    textDeviceStateLayout->addWidget(mYPosition, 1, 1);
    textDeviceStateLayout->addWidget(mXSpeed, 2, 1);
    textDeviceStateLayout->addWidget(mYSpeed, 3, 1);

  //  dashBoardLayout->addLayout(textDeviceStateLayout);

    auto *controlLayout = new QGridLayout;

    auto formLayout = new QFormLayout;
    formLayout->addRow(new QLabel("Task Ox:"), mOxTask);
    formLayout->addRow(new QLabel("Task Oy"), mOyTask);
    formLayout->addRow(new QLabel("send by button"), mModeControl);
    controlLayout->addLayout(formLayout, 0, 0);

    formLayout = new QFormLayout;
    formLayout->addRow(new QLabel("Pwm Ox:"), mPwmOx);
    formLayout->addRow(new QLabel("Pwm Oy:"), mPwmOy);
    formLayout->addRow(new QLabel("Light:"), mLightBlinking);
    controlLayout->addLayout(formLayout, 0, 1);

    auto *rbGroupLayout = new QHBoxLayout;
    mRbPwmControl->setText("PwmControl");
    rbGroupLayout->addWidget(mRbPwmControl);
    mRbTaskControl->setText("TaskControl");
    rbGroupLayout->addWidget(mRbTaskControl);
    mRbGroup->setTitle("Mode control");
    mRbGroup->setLayout(rbGroupLayout);
    controlLayout->addWidget(mRbGroup, 1, 0);
    mControlWidget->setLayout(controlLayout);
  //  mLayout->addLayout(controlLayout);

  // Настройка подключение к ком-порту
    auto *serialSettingsLayout = new QGridLayout;
    formLayout = new QFormLayout;
    formLayout->addRow(new QLabel("SerialPort:"), mSerialNameInput);
    formLayout->addRow(new QLabel("BaudRate:"), mBaudRate);
    serialSettingsLayout->addLayout(formLayout, 0, 0);
    mSerialConnect->setText("Connect");
    QObject::connect(mSerialConnect, &QPushButton::released,
                     this, &QSddSerialPortControl::serialConnect);
    serialSettingsLayout->addWidget(mSerialConnect);
    mSerialSettingsWidget->setLayout(serialSettingsLayout);

    auto *tabs = new QTabWidget;
    tabs->addTab(mControlWidget, "Control");
    tabs->addTab(mSerialSettingsWidget, "SerialSettings");
    tabs->addTab(new QWidget, "Python");
    dashBoardLayout->addWidget(box);
    dashBoardLayout->addWidget(tabs);
    mLayout->addLayout(dashBoardLayout);

    setGuiDefault();
}

void QSddSerialPortControl::setGuiDefault() {
    mXPosition->setText("undefined");
    mYPosition->setText("undefined");
    mXSpeed->setText("undefined");
    mYSpeed->setText("undefined");
}

void QSddSerialPortControl::serialConnect() {
    auto serialName = mSerialNameInput->text();
    m_pSerialPort->setPortName(serialName);
    if (!m_pSerialPort->open(QSerialPort::ReadWrite)) {
        qCritical() << "error opening comport: " << serialName;
        return;
    }

    qInfo() << "Open comport: " << serialName;
    // m_pSerialPort->write("Hello\n");
    bool isOK = false;
    int64_t baudRate = mBaudRate->text().toLong(&isOK);
    if (!isOK) {
        qCritical() << "Invalid baudrate value : " << mBaudRate->text();
    }
    m_pSerialPort->setBaudRate(baudRate);
    m_pSender.setPort(m_pSerialPort);
}

void QSddSerialPortControl::controlSettings() {
    mOxTask->setMinimum(-255);
    mOxTask->setMaximum(255);
    mOyTask->setMinimum(-255);
    mOyTask->setMaximum(255);
    mLightBlinking->setMinimum(0);
    mLightBlinking->setMaximum(1);
    mLightBlinking->setSingleStep(0.1);

    QObject::connect(mOxTask,  QOverload<int>::of(&QSpinBox::valueChanged), this, &QSddSerialPortControl::updateTaskControlValue);
    QObject::connect(mPwmOx,  QOverload<int>::of(&QSpinBox::valueChanged), this, &QSddSerialPortControl::updatePwmControlValue);
}

void QSddSerialPortControl::updateTaskControlValue(double value) {
    if (mRbTaskControl->isChecked()) {
        if (m_pSerialPort->isOpen()) {
            sdd::conn::TaskPosition task;
            task.ox = mOxTask->value();
            task.oy = mOyTask->value();
            qInfo() << "Send: Task{" << task.ox << "; " << task.oy << "}";
            m_pSender.sendTaskPosition(task);
        }
    }
}



void QSddSerialPortControl::updatePwmControlValue(double value) {
    if (mRbPwmControl->isChecked()) {
        if (m_pSerialPort->isOpen()) {
            sdd::conn::Pwm pwm;
            pwm.ox = mPwmOx->value();
            pwm.oy = mPwmOy->value();
            qInfo() << "Send: Pwm{" << pwm.ox << "; " << pwm.oy << "}";
            m_pSender.sendPwm(pwm);
        }
    }
}

void QSddSerialPortControl::serialInit() {
    m_pSender.addCallbackDataReady([this](const sdd::conn::State &state) {
        std::lock_guard lock(m_mutexStates);
        m_vStates.push_back(state);
    });
}

