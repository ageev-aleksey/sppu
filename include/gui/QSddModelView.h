//
// Created by nrx on 30.07.2020.
//

#ifndef TESTSIMULINKMODEL_QSDDMODELVIEW_H
#define TESTSIMULINKMODEL_QSDDMODELVIEW_H
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QGroupBox>
#include <QHBoxLayout>
#include "model/QSddModelExecutor.h"
#include "model/PwdInputGenerator.h"
#include "gui/QGridDimanicPlots.h"
#include "gui/QConditionalControl.h"

//TODO не проинициализированны внутренние виджеты
class QSddModelView : public QWidget
{
    Q_OBJECT
public:

    explicit QSddModelView(std::shared_ptr<QSddModelExecutor> modelConnector, QWidget *parent = nullptr);
    ~QSddModelView() noexcept override;

    std::vector<SddModel::State> getLastData();

public slots:
    void setModelInput(const SddModel::Input &input);
    void stateInGui();

signals:
    void inputUpdate(const SddModel::Input);

private slots:
    void modelRunFromGui();
    void modelStopFromGui();
    void modelResetFromGui();
    void updateModelParameters();
    void setDefaultModelParameters();
    void pwmParametersUpdate(double v = 0);

private:
    void guiInit();
    void  setDefaultGuiState();
    void updateGuiParametersValue();
    void takeModelState(SddModel::State state);
    QWidget* pwmControlInit();

    std::shared_ptr<QSddModelExecutor> mModel;

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
    int delayModeling = 0;


    QGridDimanicPlots *mPlots = new QGridDimanicPlots;
    std::mutex mMutexState;
    std::vector<SddModel::State> mState;
    QTimer mTimerGraphUpdate;

};


#endif //TESTSIMULINKMODEL_QSDDMODELVIEW_H
