//
// Created by nrx on 13.01.2021.
//

#ifndef TESTSIMULINKMODEL_QMODELOUTPUT_H
#define TESTSIMULINKMODEL_QMODELOUTPUT_H

#include "model/SddModel.h"
#include "model/QSddModelExecutor.h"

#include <QtCore>
#include <memory>

class QModelOutput : public QObject {
    Q_OBJECT
public:
    QModelOutput(std::shared_ptr<QSddModelExecutor> model, std::shared_ptr<QIODevice> outputDevice);
    ~QModelOutput() override = default;
public slots:
    void writeModelState(SddModel::State state);
private:
    std::shared_ptr<QSddModelExecutor> m_pModelExecutor;
    std::shared_ptr<QIODevice> m_pDevice;
};

#endif //TESTSIMULINKMODEL_QMODELOUTPUT_H
