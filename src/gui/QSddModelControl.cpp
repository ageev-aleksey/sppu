#include "gui/QSddModelControl.h"
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QTabWidget>

QSddModelControl::QSddModelControl(std::unique_ptr<QSddModelExecutor> mExec, QWidget *parent)
    : mModelExecutor(std::move(mExec)) {
    setParent(parent);
    setLayout(mLayout);
    guiInit();
    auto status = QObject::connect(mModelExecutor.get(), &QSddModelExecutor::modelTakeStep_pack,
                     this, &QSddModelControl::takeModelState);
    if (!status) {
        qDebug() << "Не удалось соединиться";
    }
}

std::vector<sdd::conn::State> QSddModelControl::getSddStates() {
    std::lock_guard lock(mMutexState);
    return std::move(mStates);
}

void QSddModelControl::guiInit() {
    auto *dashboardLayout = new QHBoxLayout;
    // Текстовое описание состояния модели
    auto layout = new QGridLayout;
    auto *box = new QGroupBox("Current model state");
    box->setLayout(layout);
    // layout->setAlignment(Qt::Alignment(Qt::AlignmentFlag::AlignRight));
    layout->addWidget(new QLabel("X position:"), 0, 0);
    layout->addWidget(new QLabel("Z position:"), 1, 0);
    layout->addWidget(new QLabel("X speed:"), 2, 0);
    layout->addWidget(new QLabel("Z speed:"), 3, 0);



    layout->addWidget(mXPosition, 0, 1);
    layout->addWidget(mZPosition, 1, 1);
    layout->addWidget(mXSpeed, 2, 1);
    layout->addWidget(mZSpeed, 3, 1);


    //Кнопки запуска остановки моделирования сброса и установки новых параметров модели
    layout = new QGridLayout;
    startButton->setText("start");
    stopButton->setText("stop");
    stopButton->setDisabled(true);
    resetButton->setText("reset");

    layout->addWidget(startButton, 2, 0);
    layout->addWidget(stopButton, 2, 1);
    layout->addWidget(resetButton, 2, 2);

    QObject::connect(startButton, &QPushButton::released, this, &QSddModelControl::modelRunFromGui);
    QObject::connect(stopButton, &QPushButton::released, this, &QSddModelControl::modelStopFromGui);
    QObject::connect(resetButton, &QPushButton::released, this, &QSddModelControl::modelResetFromGui);

    // Параметры модели
    // 1. начальное положение
    auto *ParametersLayout = new QGridLayout;
    // boxParameters->setLayout(ParametersLayout);
    QFormLayout *textAndSpin = new QFormLayout;
    textAndSpin->addRow(new QLabel("Ox0:"), Ox0Parameter);
    ParametersLayout->addLayout(textAndSpin, 0, 0);
    textAndSpin = new QFormLayout;
    textAndSpin->addRow(new QLabel("Oz0:"), Oz0Parameter);
    ParametersLayout->addLayout(textAndSpin, 0, 1);
    // 2. начальная скорость
    textAndSpin = new QFormLayout;
    textAndSpin->addRow(new QLabel("Vx0:"), Vx0Parameter);
    ParametersLayout->addLayout(textAndSpin, 0, 2);
    textAndSpin = new QFormLayout;
    textAndSpin->addRow(new QLabel("Vz0:"), Vz0Parameter);
    ParametersLayout->addLayout(textAndSpin, 0, 3);
    // 3. Структурная константа
    textAndSpin = new QFormLayout;
    textAndSpin->addRow(new QLabel("C1:"), ModelStructParameter);
    ParametersLayout->addLayout(textAndSpin, 1, 0);
    // 4. Константы трения
    textAndSpin = new QFormLayout;
    textAndSpin->addRow(new QLabel("C2:"), frictionParameter);
    ParametersLayout->addLayout(textAndSpin, 1, 1);
    textAndSpin = new QFormLayout;
    textAndSpin->addRow(new QLabel("C3:"), frictionLinearParameter);
    ParametersLayout->addLayout(textAndSpin, 1, 2);
    textAndSpin = new QFormLayout;
    textAndSpin->addRow(new QLabel("C4:"), frictionQuadraticParameter);
    ParametersLayout->addLayout(textAndSpin, 1, 3);
    textAndSpin = new QFormLayout;
    textAndSpin->addRow(new QLabel("modeling delay (mis):"), delayModelingField);
    ParametersLayout->addLayout(textAndSpin, 2, 0);

    applyParametersButton->setText("apply");
    ParametersLayout->addWidget(applyParametersButton, 2, 2);
    defaultParameterButton->setText("set default");
    ParametersLayout->addWidget(defaultParameterButton, 2, 3);

    QObject::connect(applyParametersButton, &QPushButton::released,
                     this, &QSddModelControl::updateModelParameters);
    QObject::connect(defaultParameterButton, &QPushButton::released,
                     this, &QSddModelControl::setDefaultModelParameters);

    boxParameters->setLayout(ParametersLayout);

    QTabWidget *paramTabs = new QTabWidget;
    paramTabs->addTab(boxParameters, "Model parameters");
    QWidget *pwmControl = pwmControlInit();
    paramTabs->addTab(pwmControl, "Online PWM Control");

    layout->addWidget(paramTabs, 0, 0, 2, 6);

    dashboardLayout->addWidget(box);
    dashboardLayout->addLayout(layout);
    mLayout->addLayout(dashboardLayout);

    setDefaultGuiState();
}

void QSddModelControl::setDefaultGuiState() {
    mXPosition->setText("undefined");
    mZPosition->setText("undefined");
    mXSpeed->setText("undefined");
    mZSpeed->setText("undefined");

    updateGuiParametersValue();
}

void QSddModelControl::updateGuiParametersValue() {
    auto modelParam = mModelExecutor->getParameters();
    Ox0Parameter->setText(QString::number(modelParam.positionOx0));
    Oz0Parameter->setText(QString::number(modelParam.positionOz0));
    Vx0Parameter->setText(QString::number(modelParam.speedOx0));
    Vz0Parameter->setText(QString::number(modelParam.speedOz0));
    ModelStructParameter->setText(QString::number(modelParam.structCoeff));
    frictionParameter->setText(QString::number(modelParam.frictionCoeff));
    frictionLinearParameter->setText(QString::number(modelParam.frictionLinearCoeff));
    frictionQuadraticParameter->setText(QString::number(modelParam.frictionQuadraticCoeff));
    delayModelingField->setText(QString::number(mModelExecutor->brakingModeling()));
}

void QSddModelControl::takeModelState(sdd::conn::State state) {
    std::lock_guard<std::mutex> lock(mMutexState);
    mStates.push_back(state);
    mXPosition->setText(QString::number(state.ox));
    mZPosition->setText(QString::number(state.oy));
    // TODO(ageev) вычисление моментальной скорости
//    mXSpeed->setText("undefined");
//    mZSpeed->setText("undefined");
}

void QSddModelControl::modelRunFromGui() {
    if (!mModelExecutor->isRun()) {
        startButton->setDisabled(true);
        stopButton->setDisabled(false);
        applyParametersButton->setDisabled(true);
        resetButton->setDisabled(true);
        boxParameters->setDisabled(true);
        pwmFrequency->setDisabled(true);
        pwmOnlineControl->setDisableChangingState(true);
        if (pwmOnlineControl->isChecked()) {
            pwdGenerator = std::make_shared<PwdInputGenerator>(
                    pwmFrequency->value(),
                    pwmMinSignalValue->value(),
                    pwmMaxSignalValue->value());
            pwmParametersUpdate();
            mModelExecutor->setInputGenerator(pwdGenerator);
        }
        mModelExecutor->run();
    }
}

void QSddModelControl::modelStopFromGui() {
    if (mModelExecutor->isRun()) {
        mModelExecutor->stop();
        startButton->setDisabled(false);
        stopButton->setDisabled(true);
        applyParametersButton->setDisabled(true);
        resetButton->setDisabled(false);
    }
}

void QSddModelControl::modelResetFromGui() {
    mModelExecutor->reset();
    startButton->setDisabled(false);
    stopButton->setDisabled(true);
    applyParametersButton->setDisabled(false);
    resetButton->setDisabled(true);
    boxParameters->setDisabled(false);
    pwmFrequency->setDisabled(false);
    pwmOnlineControl->setDisableChangingState(false);
    setDefaultGuiState();
}

void QSddModelControl::updateModelParameters() {
    SddModel::Parameters param{};
    param.positionOx0 = Ox0Parameter->text().toDouble();
    param.positionOz0 = Oz0Parameter->text().toDouble();
    param.speedOx0 = Vx0Parameter->text().toDouble();
    param.speedOz0 = Vz0Parameter->text().toDouble();
    param.structCoeff = ModelStructParameter->text().toDouble();
    param.frictionCoeff = frictionParameter->text().toDouble();
    param.frictionLinearCoeff = frictionLinearParameter->text().toDouble();
    param.frictionQuadraticCoeff = frictionQuadraticParameter->text().toDouble();
    mModelExecutor->brakingModeling(delayModelingField->text().toInt());
    mModelExecutor->setParameters(param);
}

void QSddModelControl::setDefaultModelParameters() {
//    mXPosition->setText("undefined");
//    mZPosition->setText("undefined");
//    mXSpeed->setText("undefined");
//    mZSpeed->setText("undefined");
//
//    updateGuiParametersValue();
}

// TODO(ageev) Сделать настройку скважности для отдельных осей
void QSddModelControl::pwmParametersUpdate() {
    double v = pwmDutyCycle->value();
    if (pwdGenerator) {
        pwdGenerator->setDutyCycle(v, v);
    }
}

QWidget *QSddModelControl::pwmControlInit() {
    auto layout = new QGridLayout;

    auto *label = new QFormLayout;
    pwmFrequency->setRange(1, 10000);
    label->addRow("frequency", pwmFrequency);
    layout->addLayout(label, 0, 0);

    label = new QFormLayout;
    pwmDutyCycle->setRange(0, 1);
    pwmDutyCycle->setSingleStep(0.01);
    label->addRow("duty cycle", pwmDutyCycle);
    layout->addLayout(label, 1, 0);

    label = new QFormLayout;
    pwmMinSignalValue->setRange(-10, 10);
    label->addRow("min signal", pwmMinSignalValue);
    layout->addLayout(label, 2, 0);

    label = new QFormLayout;
    pwmMaxSignalValue->setRange(-10, 10);
    label->addRow("max signal", pwmMaxSignalValue);
    layout->addLayout(label, 2, 1);

    QObject::connect(pwmFrequency, QOverload<int>::of(&QSpinBox::valueChanged), this,
                     &QSddModelControl::pwmParametersUpdate);
    QObject::connect(pwmDutyCycle, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
                     &QSddModelControl::pwmParametersUpdate);
    QObject::connect(pwmMinSignalValue, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
                     &QSddModelControl::pwmParametersUpdate);
    QObject::connect(pwmMaxSignalValue, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
                     &QSddModelControl::pwmParametersUpdate);
    pwmOnlineControl->addLayout(layout);
    return pwmOnlineControl;
}

void QSddModelControl::settingsLoad(const QSettings &settings) {
    // empty
}

void QSddModelControl::settingsStore(QSettings &settings) {
    // empty
}

// TODO (ageev) Реализовать
std::shared_ptr<sdd::conn::QIConnection> QSddModelControl::getSddConnection() {
    return nullptr;
}
