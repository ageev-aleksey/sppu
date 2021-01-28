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
#include "sdd_protocol/connect/IConnection.h"

class QSddModelSaver : public QWidget {
    Q_OBJECT
public:
    QSddModelSaver(const FormatsContainer<SddModelDescriptor> &formats,
                   std::shared_ptr<sdd::conn::IConnection> connection,
                   QWidget *parent = nullptr);
public slots:
    void addModelState(const sdd::conn::State &state);
private slots:
    void save();
private:
    QString filesTypeToString();
    std::vector<sdd::conn::State> mStates;
    FormatsContainer<SddModelDescriptor> mFormats;
    QHBoxLayout *mLayout = new QHBoxLayout;
    QPushButton *mSaveButton = new QPushButton;
    QFile mFile;
    std::shared_ptr<sdd::conn::IConnection> mSddConnection;
    // QMetaObject::Connection mChannelBufferWrite;

};
#endif //TESTSIMULINKMODEL_QSDDMODELSAVER_H
