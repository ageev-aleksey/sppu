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
#include "gui/QGridDynamicPlots.h"
#include "gui/QConditionalControl.h"
#include "gui/QISddStateWidget.h"

//TODO не проинициализированны внутренние виджеты

/**
 * Виджет отображения текущего состояния устройства
 */
class QSddView : public QWidget
{
    Q_OBJECT
public:
    explicit QSddView(QISddStateWidget *sddConnector, QWidget *parent = nullptr);
    ~QSddView() noexcept override;
    QISddStateWidget* setSddStateWidget(QISddStateWidget *sddConnector);

public slots:
    void setModelInput(const SddModel::Input &input);
    void stateInGui();

signals:
    void inputUpdate(const SddModel::Input);
private:
    void guiInit();

    QISddStateWidget *mSddConnect;


    int delayModeling = 0;

    QVBoxLayout *mLayout = new QVBoxLayout;
    QGridDynamicPlots *mPlots = new QGridDynamicPlots;
    std::mutex mMutexState;
    std::vector<sdd::conn::State> mState;
    QTimer mTimerGraphUpdate;
    sdd::conn::State mBeforeState{};

};


#endif //TESTSIMULINKMODEL_QSDDMODELVIEW_H
