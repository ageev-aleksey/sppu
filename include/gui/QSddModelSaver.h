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

class QSddModelSaver : public QWidget {
    Q_OBJECT
public:
    explicit QSddModelSaver(const FormatsContainer<SddModelDescriptor> &formats, QWidget *parent = nullptr);
public slots:
    void addModelState(const SddModel::State &state);
    void setModelInput(const TimedInput &input);
private slots:
    void save();
private:
    QString filesTypeToString();
    SddModelDescriptor mStates;
    SddModel::Input mInput{};
    FormatsContainer<SddModelDescriptor> mFormats;
    QHBoxLayout *mLayout = new QHBoxLayout;
    QPushButton *mSaveButton = new QPushButton;

};
#endif //TESTSIMULINKMODEL_QSDDMODELSAVER_H
