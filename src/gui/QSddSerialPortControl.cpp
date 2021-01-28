#include "gui/QSddSerialPortControl.h"
#include <iostream>


QSddSerialPortControl::QSddSerialPortControl(std::unique_ptr<QSerialPort> connection)
    : m_pSerialPort(std::move(connection)),  m_pSender(new sdd::conn::QSerialPortConnection)
{
    setLayout(mLayout);
    guiInit();
    controlSettings();
    serialInit();
}

std::vector<sdd::conn::State> QSddSerialPortControl::getSddStates() {
    std::lock_guard lock (m_mutexStates);
   /// return std::move(m_vStates);
    return { m_lastState };
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
    textDeviceStateLayout->addWidget(new QLabel("Diff time recv packs:"), 4, 0);
    textDeviceStateLayout->addWidget(mXPosition, 0, 1);
    textDeviceStateLayout->addWidget(mYPosition, 1, 1);
    textDeviceStateLayout->addWidget(mXSpeed, 2, 1);
    textDeviceStateLayout->addWidget(mYSpeed, 3, 1);
    textDeviceStateLayout->addWidget(mSpeedPackageValue, 4, 1);

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
    mButtonSendOptions->setText("Send");
    controlLayout->addWidget(mButtonSendOptions, 1, 1);

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
    mSpeedPackageValue->setText("undefined");
}

void QSddSerialPortControl::serialConnect() {
    auto serialName = mSerialNameInput->text();
    m_pSerialPort->setPortName(serialName);
    if (!m_pSerialPort->open(QSerialPort::ReadWrite)) {
        qCritical() << "error opening comport [" << m_pSerialPort->error() << "]: " << serialName;
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
    m_pSender->setPort(m_pSerialPort);
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

    mModeControl->setCheckState(Qt::Unchecked);
    mButtonSendOptions->setEnabled(false);
    QObject::connect(mButtonSendOptions, &QPushButton::released, this, &QSddSerialPortControl::buttonSend);
    QObject::connect(mModeControl, &QCheckBox::stateChanged, this, &QSddSerialPortControl::takeModeControl);

    QObject::connect(mLightBlinking, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                     this, &QSddSerialPortControl::blinkingUpdate);
}

void QSddSerialPortControl::updateTaskControlValue(double value) {
    if (mRbTaskControl->isChecked() && !mModeControl->isChecked()) {
        if (m_pSerialPort->isOpen()) {
            sendTaskPackage();
        }
    }
}

void QSddSerialPortControl::sendTaskPackage() {
    sdd::conn::TaskPosition task;
    task.ox = mOxTask->value();
    task.oy = mOyTask->value();
    qInfo() << "Send: Task{" << task.ox << "; " << task.oy << "}";
    m_pSender->sendTaskPosition(task);
}


void QSddSerialPortControl::updatePwmControlValue(double value) {
    if (mRbPwmControl->isChecked() && !mModeControl->isChecked()) {
        if (m_pSerialPort->isOpen()) {
            sendPwmPackage();
        }
    }
}

void QSddSerialPortControl::sendPwmPackage() {
    sdd::conn::Pwm pwm;
    pwm.ox = mPwmOx->value();
    pwm.oy = mPwmOy->value();
    qInfo() << "Send: Pwm{" << pwm.ox << "; " << pwm.oy << "}";
    m_pSender->sendPwm(pwm);
}

void QSddSerialPortControl::serialInit() {
    m_pSender->addCallbackDataReady([this](const sdd::conn::State &state) {

        {
            std::lock_guard lock(m_mutexStates);
           // m_vStates.push_back(state);
            m_lastState = state;
        }
        textUpdate(state);
    });
}

void QSddSerialPortControl::takeModeControl(int state) {
    if (state == Qt::Checked) {
        mButtonSendOptions->setEnabled(true);
    } else {
        mButtonSendOptions->setEnabled(false);
    }
}

void QSddSerialPortControl::buttonSend() {
    if (m_pSerialPort->isOpen()) {
        if (mModeControl->isChecked()) {
            if (mRbPwmControl->isChecked()) {
                sendPwmPackage();
            } else if (mRbTaskControl->isChecked()) {
                sendTaskPackage();
            }
        }
    }
}

void QSddSerialPortControl::blinkingUpdate(double value) {
    if (m_pSerialPort->isOpen()) {
        sdd::conn::Light light;
        if (value >= 1.0) {
            light.blinking = -1;
        } else {
            light.blinking = static_cast<int>(value * 10.0);
        }
        qInfo() << "Send: Light{" << light.blinking << "}";
        m_pSender->sendLight(light);
    }
}

QSddSerialPortControl::~QSddSerialPortControl() {
    m_pSerialPort->close();
}

void QSddSerialPortControl::settingsLoad(const QSettings &settings) {
    mSerialNameInput->setText(settings.value("sddSerial/devName", "").toString());
    mBaudRate->setText(settings.value("sddSerial/baudrate", "").toString());
    mPwmOx->setValue(settings.value("sddControl/pwmx", 0).toInt());
    mPwmOy->setValue(settings.value("sddControl/pwmy", 0).toInt());
    mOxTask->setValue(settings.value("sddControl/taskx", 0).toInt());
    mOyTask->setValue(settings.value("sddControl/tasky", 0).toInt());
}

void QSddSerialPortControl::settingsStore(QSettings &settings) {
    settings.setValue("sddSerial/devName", mSerialNameInput->text());
    settings.setValue("sddSerial/baudrate", mBaudRate->text());
    settings.setValue("sddControl/pwmx", mPwmOx->text());
    settings.setValue("sddControl/pwmy", mPwmOy->text());
    settings.setValue("sddControl/taskx", mOxTask->text());
    settings.setValue("sddControl/tasky", mOyTask->text());
}


void QSddSerialPortControl::textUpdate(const sdd::conn::State &state) {
    auto time = std::chrono::steady_clock::now();
    mXPosition->setText(QString::number(state.ox));
    mYPosition->setText(QString::number(state.oy));
    auto elapsedTime = time-mBeforeTimePackageGet;
    mSpeedPackageValue->setText(QString::number(std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count()));
    mBeforeTimePackageGet = time;
    mXSpeed->setText(QString::number(
            (mBeforePackageValue.ox - state.ox) /
                    (std::chrono::duration_cast<std::chrono::seconds>(elapsedTime).count() + 0.1)));
    mYSpeed->setText(QString::number((mBeforePackageValue.oy - state.oy) /
                                     (std::chrono::duration_cast<std::chrono::seconds>(elapsedTime).count() + 0.1)));
    mBeforePackageValue = state;
}

std::shared_ptr<sdd::conn::IConnection> QSddSerialPortControl::getSddConnection() {
    return m_pSender;
}

