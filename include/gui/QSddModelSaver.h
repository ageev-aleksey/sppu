//
// Created by nrx on 04.08.2020.
//

#ifndef TESTSIMULINKMODEL_QSDDMODELSAVER_H
#define TESTSIMULINKMODEL_QSDDMODELSAVER_H
#include <QtCore>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include "model/parser/SddModelDescriptor.h"
#include "format/FormatsContainer.h"
#include "sdd_protocol/connect/States.h"
#include "sdd_protocol/connect/QIConnection.h"

class QSddModelSaver : public QWidget {
    Q_OBJECT
public:
    QSddModelSaver(const FormatsContainer<sdd::conn::State> &formats,
                   std::shared_ptr<sdd::conn::QIConnection> connection,
                   QWidget *parent = nullptr);
    ~QSddModelSaver() override;
public slots:
    void addModelState(const sdd::conn::State &state);
private slots:
    void save();
private:
    void fileWrite();
    QString filesTypeToString();
    std::vector<sdd::conn::State> mStates;
    FormatsContainer<sdd::conn::State> mFormats;
    QHBoxLayout *mLayout = new QHBoxLayout;
    QPushButton *mSaveButton = new QPushButton;
    QFile mFile;
    std::shared_ptr<sdd::conn::QIConnection> mSddConnection;
    // QMetaObject::Connection mChannelBufferWrite;

};
#endif //TESTSIMULINKMODEL_QSDDMODELSAVER_H
