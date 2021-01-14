#include "model/QModelOutput.h"
#include "sdd_protocol/StatePackage.h"
#include <math.h>

static const double PI =  3.14;
static const double CORNENR45 = std::cos(PI);
static const double INTEGER_CORNER45 = 255.0;
static const double INTEGER_PWM = 255.0;

QModelOutput::QModelOutput(std::shared_ptr<QSddModelExecutor> model,
                           std::shared_ptr<QIODevice> outputDevice)
: m_pModelExecutor(std::move(model)), m_pDevice(std::move(outputDevice))
{

    QObject::connect(m_pModelExecutor.get(), &QSddModelExecutor::modelTakeStep,
                     this, &QModelOutput::writeModelState);
    m_pModelExecutor->run();
}

static short convertCorner(double value) {
    return static_cast<short>(value*INTEGER_CORNER45/CORNENR45);
}

static short convertPwm(double value) {
    return static_cast<short>(value*INTEGER_PWM);
}

void QModelOutput::writeModelState(SddModel::State state) {
    sdd::StatePackage statePackage;
    // TODO (?)
    statePackage.setOX(convertCorner(state.positionOx));
    statePackage.setOY(convertCorner(state.positionOz));
    //////////////////////
    statePackage.setPositionX(convertCorner(state.positionOx));
    statePackage.setPositionX(convertCorner(state.positionOz));
    statePackage.setPWMX(convertPwm(state.oxSignal));
    statePackage.setPWMY(convertPwm(state.ozSignal));
    m_pDevice->write(&statePackage.toBinary()[0]);
}