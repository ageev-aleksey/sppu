//
// Created by nrx on 04.08.2020.
//

#ifndef TESTSIMULINKMODEL_QSDDMODELEXECUTOR_H
#define TESTSIMULINKMODEL_QSDDMODELEXECUTOR_H
#include <QtCore>
#include "model/SddModel.h"
#include "model/InputGenerator.h"
#include "sdd_protocol/connect/IConnection.h"
#include <atomic>
#include <thread>

/**
 * Класс выполняющий организацию связи модели sdd с системой сообщений QT,
 * а так же запуск моделирования в отдельном пототке.
 */
class QSddModelExecutor : public QObject, public sdd::conn::IConnection {
    Q_OBJECT
public:
    explicit QSddModelExecutor(std::unique_ptr<SddModel> model);
    SddModel::Parameters getParameters();
    SddModel::Input getInput();
    bool isRun();
    void brakingModeling(long milliseconds);
    long brakingModeling();
    std::shared_ptr<InputGenerator> setInputGenerator(std::shared_ptr<InputGenerator> generator);
    std::shared_ptr<InputGenerator> resetInputGenerator();
    void sendLight(sdd::conn::Light package) override;
    void sendMode(sdd::conn::Mode package) override;
    void sendTaskPosition(sdd::conn::TaskPosition task) override;
public slots:
    void setParameters(const SddModel::Parameters &parameters);
    void setParametersDefault();
    void setInput(const SddModel::Input &input);
    void run();
    void stop();
    void reset();
signals:
    void receiveStatePackage(sdd::conn::State state);
    void parametersUpdate(SddModel::Parameters parameters);
    void inputUpdate(SddModel::Input input);
    void modelTakeStep(SddModel::State state);
    void modelTakeStep_pack(sdd::conn::State state);
    void modelRun();
    void modelStop();
    void modelReset();
private:
    void worker_thread();
    sdd::conn::State makePackageState();

    std::shared_ptr<SddModel> mModel;
    std::atomic<bool> mIsRun = false;
    std::atomic<long> mBreakingMilliseconds = 0;

    std::mutex mMutexUpdInputModel;
    std::shared_ptr<InputGenerator> mInputGenerator = nullptr;
    std::atomic<SddModel::State> mCurrentState{};
    bool mIsNewInput = false;
    SddModel::Input mNewInput{};
    std::function<Handle> m_dataReceived;

    std::thread mThread;

};

Q_DECLARE_METATYPE(SddModel::Parameters)
Q_DECLARE_METATYPE(SddModel::Input);
Q_DECLARE_METATYPE(SddModel::State);
#endif //TESTSIMULINKMODEL_QSDDMODELEXECUTOR_H
