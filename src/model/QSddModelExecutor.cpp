#include "model/QSddModelExecutor.h"
#include "sdd_protocol/connect/States.h"

#include <utility>
// TODO(ageev) применить декоратор для конвертации из одной структуры в другую

QSddModelExecutor::QSddModelExecutor(std::unique_ptr<SddModel> model)
    : mModel(std::move(model)), m_dataReceived(nullptr)
{
    qRegisterMetaType<SddModel::Input>("Input");
    qRegisterMetaType<SddModel::Parameters>("Parameters");
    qRegisterMetaType<SddModel::State>("StatePackage");
    qRegisterMetaType<sdd::conn::State>("sdd::conn::State");
}

void QSddModelExecutor::setParameters(const SddModel::Parameters &parameters) {
    if(!mModel->isInit()) {
        mModel->setParameters(parameters);
        emit parametersUpdate(parameters);
    }
}

void QSddModelExecutor::setInput(const SddModel::Input &input) {
    std::lock_guard<std::mutex> lock(mMutexUpdInputModel);
    mNewInput = input;
    mIsNewInput = true;
}

void QSddModelExecutor::run() {
    if(mIsRun) {
        // throw std::runtime_error("model is already started");
        return;
    }
    if(!mModel->isInit()) {
        mModel->init();
    }
    mIsRun = true;
    mThread = std::thread(&QSddModelExecutor::worker_thread, this);
    emit modelRun();
}

void QSddModelExecutor::stop() {
    mIsRun = false;
    if (mThread.joinable()) {
        mThread.join();
        emit modelStop();
    }
}

int64_t dSecondsToINanoseconds(double seconds) {
    auto val = static_cast<int64_t>(seconds * 1000000000ull);
    return val;
}

void QSddModelExecutor::worker_thread() {
    int nstep = 0;
    while(mIsRun) {
        nstep++;
        {
            std::lock_guard<std::mutex> lock(mMutexUpdInputModel);
            if(mInputGenerator != nullptr) {
                mNewInput = mInputGenerator->generate(mCurrentState);
                mModel->setInput(mNewInput);
                emit inputUpdate(mNewInput);
            } else {
                if(mIsNewInput) {
                    mIsNewInput = false;
                    mModel->setInput(mNewInput);
                    emit inputUpdate(mNewInput);
                }
            }

        }

        mCurrentState = mModel->step();

        sdd::conn::State stateValue = makePackageState();

        if (nstep == mBreakingMilliseconds) {
            nstep = 0;
            if (m_dataReceived) {
                m_dataReceived(stateValue);
            }

            emit modelTakeStep(mCurrentState);
            emit modelTakeStep_pack(stateValue);
        }

        //QCoreApplication::processEvents();
//        if (mBreakingMilliseconds > 0) {
//            std::this_thread::sleep_for(std::chrono::milliseconds(mBreakingMilliseconds));
//        }
    }
}


SddModel::Parameters QSddModelExecutor::getParameters() {
    return mModel->getParameters();
}

SddModel::Input QSddModelExecutor::getInput() {
    return mNewInput;
}

void QSddModelExecutor::setParametersDefault() {
    if(!mModel->isInit()) {
        mModel->setParametersDefault();
        auto p = mModel->getParameters();
        emit parametersUpdate(p);
    }
}

void QSddModelExecutor::reset() {
    if(!mIsRun) {
        mModel->reset();
        emit modelReset();
    }
}

bool QSddModelExecutor::isRun() {
    return mIsRun;
}

std::shared_ptr<InputGenerator> QSddModelExecutor::setInputGenerator(std::shared_ptr<InputGenerator> generator) {
    std::shared_ptr<InputGenerator> tmp = std::move(mInputGenerator);
    mInputGenerator = std::move(generator);
    return tmp;
}

std::shared_ptr<InputGenerator> QSddModelExecutor::resetInputGenerator() {
    return setInputGenerator(nullptr);
}


void QSddModelExecutor::sendLight(sdd::conn::Light package) {
    // empty. Not effect
}

void QSddModelExecutor::sendMode(sdd::conn::Mode package) {
    if (package.isOn) {
        run();
    } else {
        stop();
    }
}

void QSddModelExecutor::sendTaskPosition(sdd::conn::TaskPosition task) {
    // empty. Not effect
}

sdd::conn::State QSddModelExecutor::makePackageState() {
    auto modelState = mCurrentState.load();
    sdd::conn::State stateValue{};
    // TODO(ageev) выполнить преобразование из double в short int
    stateValue.ox = modelState.positionOx*255;
    stateValue.oy = modelState.positionOz*255;
    stateValue.pwm.ox = modelState.oxSignal;
    stateValue.pwm.oy = modelState.ozSignal;
    stateValue.task.ox = 0;
    stateValue.task.oy = 0;
    stateValue.time = std::chrono::steady_clock::time_point(
            std::chrono::steady_clock::duration(dSecondsToINanoseconds(modelState.time))
    );
    return stateValue;
}

void QSddModelExecutor::brakingModeling(long milliseconds) {
    mBreakingMilliseconds  = milliseconds;
}

long QSddModelExecutor::brakingModeling() {
    return mBreakingMilliseconds;
}

void QSddModelExecutor::sendPwm(sdd::conn::Pwm pwm) {
    // empty
}



