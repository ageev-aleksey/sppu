//
// Created by nrx on 04.08.2020.
//

#ifndef TESTSIMULINKMODEL_QSDDMODELSAVER_H
#define TESTSIMULINKMODEL_QSDDMODELSAVER_H
#include <QtCore>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include "gui/model_saver/SavingData.h"
#include "model/parser/SddModelDescriptor.h"
#include "format/FormatsContainer.h"
#include "sdd_protocol/connect/States.h"
#include "sdd_protocol/connect/QIConnection.h"
#include <unordered_map>
#include <string>

class QModelDataGetter;

class QSddModelSaver : public QWidget {
    Q_OBJECT
public:

    QSddModelSaver(const FormatsContainer<SavingData> &formats,
                   std::shared_ptr<QModelDataGetter> getter,
                   QWidget *parent = nullptr);
    void setGetter(std::shared_ptr<QModelDataGetter> getter);
    ~QSddModelSaver() override;
public slots:
    void addModelState(const SavingData &state);
private slots:
    void save();
private:
    void fileWrite();
    QString filesTypeToString();
    std::vector<SavingData> mStates;
    FormatsContainer<SavingData> mFormats;
    QHBoxLayout *mLayout = new QHBoxLayout;
    QPushButton *mSaveButton = new QPushButton;
    QFile mFile;
    std::shared_ptr<QModelDataGetter> mSddConnection;
    QMetaObject::Connection mGetterConnection;
    // QMetaObject::Connection mChannelBufferWrite;

};
#endif //TESTSIMULINKMODEL_QSDDMODELSAVER_H
