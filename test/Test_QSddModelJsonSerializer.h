//
// Created by nrx on 06.08.2020.
//

#ifndef TESTSIMULINKMODEL_TEST_QSDDMODELJSONSERIALIZER_H
#define TESTSIMULINKMODEL_TEST_QSDDMODELJSONSERIALIZER_H
#include <iostream>
#include <QtCore>
#include <QtTest>
#include "model/parser/QSddModelJsonSerializer.h"

class Test_QSddModelJsonSerializer : public QObject {
    Q_OBJECT
private slots:
    void inputSerilaize() {
        SddModelSerializer *s = new QSddModelJsonSerializer;
        SddModel::Input i{};
        QString json = s->inputSerialize(i);
        SddModel::Input res = s->inputDeserialize(json);
        QCOMPARE(res, i);
    }
};

#endif //TESTSIMULINKMODEL_TEST_QSDDMODELJSONSERIALIZER_H
