#include "gui/QSddModelView.h"
#include <QGroupBox>
#include <QPushButton>


QSddModelView::QSddModelView(std::shared_ptr<QSddModelExecutor> model, QWidget *parent)
: QWidget(parent), mModel(std::move(model)) {
    setLayout(mLayout);
    guiInit();
    QObject::connect(&mTimerGraphUpdate, &QTimer::timeout, this, &QSddModelView::stateInGui);
    QObject::connect(&(*mModel), &QSddModelExecutor::modelTakeStep, this, &QSddModelView::takeModelState);
    mTimerGraphUpdate.start(50);
}


QSddModelView::~QSddModelView() noexcept {
    // TODO(aleksey) 1
}




void QSddModelView::guiInit() {
    //Графики состояния модели
    QGridDimanicPlots::Property p;
    p.limits[0].keyType = QGridDimanicPlots::Property::Limits::Type::WINDOW;
    p.limits[0].valueType = QGridDimanicPlots::Property::Limits::Type::AUTO;
    p.limits[0].value = {-1, 1};  // {-1, 1};
    p.limits[0].key = {10, 0};  // {0, 0.5};
    auto *plot = mPlots->addPlot(0, 0, &p);
    plot->plotLayout()->insertRow(0);
    plot->plotLayout()->addElement(0, 0,
                                   new QCPTextElement(plot, "Ox position", QFont("sans", 12, QFont::Weight::Bold)));
    plot = mPlots->addPlot(0, 1, &p);
    plot->plotLayout()->insertRow(0);
    plot->plotLayout()->addElement(0, 0,
                                   new QCPTextElement(plot, "Ox speed", QFont("sans", 12, QFont::Weight::Bold)));
    plot = mPlots->addPlot(1, 0, &p);
    plot->plotLayout()->insertRow(0);
    plot->plotLayout()->addElement(0, 0,
                                   new QCPTextElement(plot, "Oz position", QFont("sans", 12, QFont::Weight::Bold)));
    plot = mPlots->addPlot(1, 1, &p);
    plot->plotLayout()->insertRow(0);
    plot->plotLayout()->addElement(0, 0,
                                   new QCPTextElement(plot, "Oz speed", QFont("sans", 12, QFont::Weight::Bold)));

    plot = mPlots->addPlot(0, 2, &p);
    plot->plotLayout()->insertRow(0);
    plot->plotLayout()->addElement(0, 0,
                                   new QCPTextElement(plot, "Ox input", QFont("sans", 12, QFont::Weight::Bold)));

    plot = mPlots->addPlot(1, 2, &p);
    plot->plotLayout()->insertRow(0);
    plot->plotLayout()->addElement(0, 0,
                                   new QCPTextElement(plot, "Oz input", QFont("sans", 12, QFont::Weight::Bold)));
    mLayout->addWidget(mPlots, 1);

    auto *dashboardLayout = new QHBoxLayout;

    //Тектостове описание состояния модели
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

    QObject::connect(startButton, &QPushButton::released, this, &QSddModelView::modelRunFromGui);
    QObject::connect(stopButton, &QPushButton::released, this, &QSddModelView::modelStopFromGui);
    QObject::connect(resetButton, &QPushButton::released, this, &QSddModelView::modelResetFromGui);

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

    QObject::connect(applyParametersButton, &QPushButton::released, this, &QSddModelView::updateModelParameters);
    QObject::connect(defaultParameterButton, &QPushButton::released, this, &QSddModelView::setDefaultModelParameters);

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

void QSddModelView::modelRunFromGui() {
    if (!mModel->isRun()) {
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
            mModel->setInputGenerator(pwdGenerator);
        }
        mModel->run();
    }
}

void QSddModelView::modelStopFromGui() {
    if (mModel->isRun()) {
        mModel->stop();
        startButton->setDisabled(false);
        stopButton->setDisabled(true);
        applyParametersButton->setDisabled(true);
        resetButton->setDisabled(false);
    }
}


void QSddModelView::modelResetFromGui() {
    if (!mModel->isRun()) {
        mModel->reset();
        startButton->setDisabled(false);
        stopButton->setDisabled(true);
        applyParametersButton->setDisabled(false);
        resetButton->setDisabled(true);
        boxParameters->setDisabled(false);
        pwmFrequency->setDisabled(false);
        pwmOnlineControl->setDisableChangingState(false);
        mPlots->plot(0, 0)->graph(0)->data()->clear();
        mPlots->plot(0, 1)->graph(0)->data()->clear();
        mPlots->plot(0, 2)->graph(0)->data()->clear();
        mPlots->plot(1, 0)->graph(0)->data()->clear();
        mPlots->plot(1, 1)->graph(0)->data()->clear();
        mPlots->plot(1, 2)->graph(0)->data()->clear();
        mPlots->allReplot();
        setDefaultGuiState();
    }
}

void QSddModelView::stateInGui() {
    std::vector<SddModel::State> state;
    {
        std::lock_guard<std::mutex> lock(mMutexState);
        state = std::move(mState);
    }
    if (!state.empty()) {
        auto &endState = *(--state.end());
        auto &centerState = state[state.size()/2];
        auto &beginState = state[0];

        mPlots->addPlotData(0, 0, 0, beginState.time, beginState.positionOx);
        mPlots->addPlotData(0, 0, 0, centerState.time, centerState.positionOx);
        mPlots->addPlotData(0, 0, 0, endState.time, endState.positionOx);

        mPlots->addPlotData(0, 1, 0, beginState.time, beginState.speedOx);
        mPlots->addPlotData(0, 1, 0, centerState.time, centerState.speedOx);
        mPlots->addPlotData(0, 1, 0, endState.time, endState.speedOx);

        mPlots->addPlotData(1, 0, 0, beginState.time, beginState.positionOz);
        mPlots->addPlotData(1, 0, 0, centerState.time, centerState.positionOz);
        mPlots->addPlotData(1, 0, 0, endState.time, endState.positionOz);

        mPlots->addPlotData(1, 1, 0, beginState.time, beginState.speedOz);
        mPlots->addPlotData(1, 1, 0, centerState.time, centerState.speedOz);
        mPlots->addPlotData(1, 1, 0, endState.time, endState.speedOz);

        mPlots->addPlotData(0, 2, 0, beginState.time, beginState.IOx);
        mPlots->addPlotData(0, 2, 0, centerState.time, centerState.IOx);
        mPlots->addPlotData(0, 2, 0, endState.time, endState.IOx);

        mPlots->addPlotData(1, 2, 0, beginState.time, beginState.IOz);
        mPlots->addPlotData(1, 2, 0, centerState.time, centerState.IOz);
        mPlots->addPlotData(1, 2, 0, endState.time, endState.IOz);



        mPlots->allReplot();

        mXPosition->setText(QString::number(endState.positionOx));
        mZPosition->setText(QString::number(endState.positionOz));
        mXSpeed->setText(QString::number(endState.speedOx));
        mZSpeed->setText(QString::number(endState.speedOz));
    }

//    if (!state.empty()) {
//        for (const auto &el : state) {
//            mPlots->addPlotData(0, 0, 0, el.time, el.positionOx);
//            mPlots->addPlotData(0, 1, 0, el.time, el.speedOx);
//            mPlots->addPlotData(1, 0, 0, el.time, el.positionOz);
//            mPlots->addPlotData(0, 2, 0, el.time, el.IOx);
//            mPlots->addPlotData(1, 2, 0, el.time, el.IOz);
//            mPlots->allReplot();
//        }
//        mXPosition->setText(QString::number(state.back().positionOx));
//        mZPosition->setText(QString::number(state.back().positionOz));
//        mXSpeed->setText(QString::number(state.back().speedOx));
//        mZSpeed->setText(QString::number(state.back().speedOz));
//    }
}


void QSddModelView::updateModelParameters() {
    SddModel::Parameters param{};
    param.positionOx0 = Ox0Parameter->text().toDouble();
    param.positionOz0 = Oz0Parameter->text().toDouble();
    param.speedOx0 = Vx0Parameter->text().toDouble();
    param.speedOz0 = Vz0Parameter->text().toDouble();
    param.structCoeff = ModelStructParameter->text().toDouble();
    param.frictionCoeff = frictionParameter->text().toDouble();
    param.frictionLinearCoeff = frictionLinearParameter->text().toDouble();
    param.frictionQuadraticCoeff = frictionQuadraticParameter->text().toDouble();
    delayModeling = delayModelingField->text().toInt();
    mModel->setParameters(param);
}

void QSddModelView::setDefaultGuiState() {
    mXPosition->setText("undefined");
    mZPosition->setText("undefined");
    mXSpeed->setText("undefined");
    mZSpeed->setText("undefined");

    updateGuiParametersValue();
}

void QSddModelView::updateGuiParametersValue() {
    auto modelParam = mModel->getParameters();
    Ox0Parameter->setText(QString::number(modelParam.positionOx0));
    Oz0Parameter->setText(QString::number(modelParam.positionOz0));
    Vx0Parameter->setText(QString::number(modelParam.speedOx0));
    Vz0Parameter->setText(QString::number(modelParam.speedOz0));
    ModelStructParameter->setText(QString::number(modelParam.structCoeff));
    frictionParameter->setText(QString::number(modelParam.frictionCoeff));
    frictionLinearParameter->setText(QString::number(modelParam.frictionLinearCoeff));
    frictionQuadraticParameter->setText(QString::number(modelParam.frictionQuadraticCoeff));
    delayModelingField->setText(QString::number(delayModeling));
}

void QSddModelView::setDefaultModelParameters() {
    mModel->setParametersDefault();
    updateGuiParametersValue();
}



void QSddModelView::setModelInput(const SddModel::Input &input) {
    mModel->setInput(input);
}

void QSddModelView::takeModelState(SddModel::State state) {
    {
        std::lock_guard<std::mutex> lock(mMutexState);
        mState.push_back(state);
    }
    // TODO(ageev) убрать. Задержка ьормозит GUI а не поток исполнения модели
    int valueDelay = delayModelingField->text().toInt();
    if (valueDelay != 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(valueDelay));
    }
}


QWidget* QSddModelView::pwmControlInit() {
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
                     &QSddModelView::pwmParametersUpdate);
    QObject::connect(pwmDutyCycle, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
                     &QSddModelView::pwmParametersUpdate);
    QObject::connect(pwmMinSignalValue, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
                     &QSddModelView::pwmParametersUpdate);
    QObject::connect(pwmMaxSignalValue, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
                     &QSddModelView::pwmParametersUpdate);
    pwmOnlineControl->addLayout(layout);
    return pwmOnlineControl;
}


// TODO(aleksey) Сделать установку скважности шим по двум осям отдельно
void QSddModelView::pwmParametersUpdate(double) {
    double v = pwmDutyCycle->value();
    if (pwdGenerator) {
        pwdGenerator->setDutyCycle(v, v);
    }
}


