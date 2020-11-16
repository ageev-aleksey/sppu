#include "model/QSddModelExecutor.h"

#include <utility>


QSddModelExecutor::QSddModelExecutor(std::unique_ptr<SddModel> model) : mModel(std::move(model)) {
    qRegisterMetaType<SddModel::Input>("Input");
    qRegisterMetaType<SddModel::Parameters>("Parameters");
    qRegisterMetaType<SddModel::State>("State");
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
        throw std::runtime_error("model is already started");
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
    mThread.join();
    emit modelStop();
}

void QSddModelExecutor::worker_thread() {
    while(mIsRun) {

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

        emit modelTakeStep(mCurrentState);
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
    std::shared_ptr<InputGenerator> tmp = mInputGenerator;
    mInputGenerator = std::move(generator);
    return tmp;
}

std::shared_ptr<InputGenerator> QSddModelExecutor::resetInputGenerator() {
    return setInputGenerator(nullptr);
}


