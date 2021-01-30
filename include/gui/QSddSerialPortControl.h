//
// Created by nrx on 17.01.2021.
//

#ifndef SDDCLIENT_QSDDSERIALPORTCONTROL_H
#define SDDCLIENT_QSDDSERIALPORTCONTROL_H
#include "gui/QISddStateWidget.h"
#include "sdd_protocol/connect/QSerialPortConnection.h"
#include <QtWidgets>

#include <memory>


class QSddSerialPortControl : public QISddStateWidget {
    Q_OBJECT
public:
    explicit QSddSerialPortControl(std::unique_ptr<QSerialPort> SerialPort);
    ~QSddSerialPortControl() override;
    std::vector<sdd::conn::State> getSddStates() override;
    void settingsLoad(const QSettings &settings) override;
    void settingsStore(QSettings &settings) override;
    std::shared_ptr<sdd::conn::QIConnection> getSddConnection();
private:
    void guiInit();
    void setGuiDefault();
    void controlSettings();
private slots:
    void addSddState(const sdd::conn::State &state);
    void serialConnect();
    void updateTaskControlValue(double value);
    void updatePwmControlValue(double value);
    void takeModeControl(int state);
    void buttonSend();
    void blinkingUpdate(double value);
private:
    void textUpdate(const sdd::conn::State &state);
    void serialInit();
    std::shared_ptr<QSerialPort> m_pSerialPort;
    std::shared_ptr<sdd::conn::QSerialPortConnection> m_pSender;


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
    // 4. Настройка режима управления, через задание или скважность ШИМ
    QGroupBox *mRbGroup = new QGroupBox;
    QRadioButton *mRbPwmControl = new QRadioButton;
    QRadioButton *mRbTaskControl = new QRadioButton;
    QCheckBox *mModeControl = new QCheckBox;
    QPushButton *mButtonSendOptions = new QPushButton;
    // Вкладка настройки КомПорта
    QWidget *mSerialSettingsWidget = new QWidget;
    QLineEdit *mSerialNameInput = new QLineEdit; /// Сптсок доступных портов
    QLineEdit *mBaudRate = new QLineEdit; /// Настройка скорости передачи
    QPushButton *mSerialConnect = new QPushButton; /// Подключение к ком-порту
    // 1. Скорость передачи бит
    QLabel *mSpeedPackageLabel = new QLabel;
    QLabel *mSpeedPackageValue = new QLabel;
    std::chrono::steady_clock::time_point mBeforeTimePackageGet = std::chrono::steady_clock::now();
    sdd::conn::State mBeforePackageValue{};

    std::mutex m_mutexStates;
    //std::vector<sdd::conn::State> m_vStates;
    sdd::conn::State m_lastState{};
    bool m_isGet = false;

    void sendTaskPackage();

    void sendPwmPackage();
};

#endif //SDDCLIENT_QSDDSERIALPORTCONTROL_H
