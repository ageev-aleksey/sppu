#include "model/SddModel.h"
#include "sdd_model.h"




SddModel::SddModel() {
    mIsInit = false;
    pImpl = new sdd_modelModelClass;
}

SddModel::~SddModel() {
    pImpl->terminate();
    delete pImpl;
}

SddModel::State SddModel::step() {
    pImpl->step();
    auto s = pImpl->getExternalOutputs();
    State state;
    state.positionOx = s.positionOx;
    state.positionOz = s.positionOz;
    state.speedOx = s.speedOX;
    state.speedOz = s.speedOz;
    state.IOx = s.IOx;
    state.IOz = s.IOz;
    state.oxSignal = mCurentInput.pwmOx;
    state.ozSignal = mCurentInput.pwmOz;
    auto runtime = pImpl->getRTM();
    time_T stepSize = runtime->Timing.stepSize0;
    uint32_T nStep = runtime->Timing.clockTick0;
    state.time = stepSize*nStep;
    return state;
}

void SddModel::setInput(const Input &input) {
    mCurentInput = input;
    auto *modelIn = new ExtU_sdd_model_T;
    modelIn->pwmOx = input.pwmOx;
    modelIn->pwmOz = input.pwmOz;
    pImpl->setExternalInputs(modelIn);
}

SddModel::Parameters SddModel::getParameters() {
    auto &p = sdd_modelModelClass::getModelParameters();
    Parameters param{};
    param.positionOx0 = p.degX0;
    param.positionOz0 = p.degZ0;
    param.speedOx0 = p.VX0;
    param.speedOz0 = p.VZ0;
    param.frictionCoeff = p.frictionCoef;
    param.frictionLinearCoeff = p.frictionLinearCoef;
    param.frictionQuadraticCoeff = p.frictionQuadraticCoef;
    param.structCoeff = p.StructModelConst;
    return param;
}

void SddModel::setParameters(const Parameters &parameters) {
    if(mIsInit) {
        throw std::runtime_error("Model is already initialized. Reset model, then set parameters and again initialize");
    }
    auto &p = sdd_modelModelClass::getModelParameters();
    p.degX0 = parameters.positionOx0;
    p.degZ0 = parameters.positionOz0;
    p.VX0 = parameters.speedOx0;
    p.VZ0 = parameters.speedOz0;
    p.frictionCoef = parameters.frictionCoeff;
    p.frictionLinearCoef = parameters.frictionLinearCoeff;
    p.frictionQuadraticCoef = parameters.frictionQuadraticCoeff;
    p.StructModelConst = parameters.structCoeff;
}

void SddModel::init() {
    pImpl->initialize();
    mIsInit = true;
}

void SddModel::reset() {
    delete pImpl;
    pImpl = new sdd_modelModelClass;
    mIsInit = false;
}

bool SddModel::isInit() {
    return mIsInit;
}

void SddModel::setParametersDefault() {
    sdd_modelModelClass::backToDefaultModelParameters();
}



