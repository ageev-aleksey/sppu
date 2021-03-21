//
// Created by nrx on 17.01.2021.
//

#ifndef SDDCLIENT_QSDDMODELCONTROLL_H
#define SDDCLIENT_QSDDMODELCONTROLL_H

#include "gui/QISddStateWidget.h"
#include "gui/QConditionalControl.h"
#include "model/QSddModelExecutor.h"
#include "model/PwdInputGenerator.h"
#include <QtCore>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <memory>

class QSddModelControl : public QISddStateWidget {
    Q_OBJECT
public:
    explicit QSddModelControl(std::unique_ptr<QSddModelExecutor> mExec, QWidget *parent = nullptr);
    std::vector<sdd::conn::State> getSddStates() override;
    void settingsLoad(const QSettings &settings) override;
    void settingsStore(QSettings &settings) override;
    std::shared_ptr<sdd::conn::QIConnection> getSddConnection() override;
private slots:
    void modelRunFromGui();
    void modelStopFromGui();
    void modelResetFromGui();
    void updateModelParameters();
    void setDefaultModelParameters();
    void pwmParametersUpdate();
private:
    void guiInit();
    void  setDefaultGuiState();
    void updateGuiParametersValue();
    void takeModelState(sdd::conn::State state);
    QWidget* pwmControlInit();

    std::unique_ptr<QSddModelExecutor> mModelExecutor;

    // Вывод текущего состояние модели
    QVBoxLayout *mLayout = new QVBoxLayout;
    QLabel *mXPosition = new QLabel;
    QLabel *mZPosition = new QLabel;
    QLabel *mXSpeed = new QLabel;
    QLabel *mZSpeed = new QLabel;

    // Управление процессом моделирования
    QPushButton *startButton = new QPushButton; //< Запупс моделирования
    QPushButton *stopButton = new QPushButton;//< остановка (пауза) моделирования
    QPushButton *resetButton = new QPushButton;//< сброс сотояния модели до моделирования
    QPushButton *applyParametersButton = new QPushButton; //< приминения новых параметров модели. Доступна перед началом моделирования или после сброса модели
    QPushButton *defaultParameterButton = new QPushButton;//< сброс параметров модели на значение по умолчанию

    // Панель для управления входом в модель во время моделирования. Входом является сигнал ШИМ.
    QConditionalControl *pwmOnlineControl = new QConditionalControl;
    QSpinBox *pwmFrequency = new QSpinBox; //< Частота шим. Не может быть больше чем позволяет размер шага моделирования
    QDoubleSpinBox *pwmDutyCycle = new QDoubleSpinBox; //< Скважность шим. От 0 до 1;
    QDoubleSpinBox *pwmMaxSignalValue = new QDoubleSpinBox; //< значени сигнала при логической 1.
    QDoubleSpinBox *pwmMinSignalValue = new QDoubleSpinBox; //< значение сигнала при логическом 0.
    std::shared_ptr<PwdInputGenerator> pwdGenerator = nullptr;

    // Поля для настройки параметров модели перед моделированием
    QWidget *boxParameters = new QWidget;
    QLineEdit *Ox0Parameter = new QLineEdit; //< Начальное положение по оси Ox.
    QLineEdit *Oz0Parameter  = new QLineEdit; //< Начальное положение по оси Oz.
    QLineEdit *Vx0Parameter  = new QLineEdit; //< Начальная скорость по оси Ox.
    QLineEdit *Vz0Parameter  = new QLineEdit; //< Начальная скорость по оси Oz.
    QLineEdit *ModelStructParameter  = new QLineEdit; //< Параметр описывающий визическую структуру модели.
    QLineEdit *frictionParameter  = new QLineEdit; //< Общий коэффициент силы трения.
    QLineEdit *frictionLinearParameter  = new QLineEdit; //< Коэффициент при линеном компоненте силы трения.
    QLineEdit *frictionQuadraticParameter  = new QLineEdit; //< Коэффициент при квадратичном компоненте силы трения.
    QLineEdit *delayModelingField = new QLineEdit; //< Параметр управлющий скоростью моделирования.
    // между шагами моделирования.

    //Хранение состояние модели, до тех пор пока его не заберут
    std::vector<sdd::conn::State> mStates;
    std::mutex mMutexState;

};

#endif //SDDCLIENT_QSDDMODELCONTROLL_H
