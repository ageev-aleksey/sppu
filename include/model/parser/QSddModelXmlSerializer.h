//
// Created by nrx on 04.08.2020.
//

#ifndef TESTSIMULINKMODEL_QSDDMODELXMLSERIALIZER_H
#define TESTSIMULINKMODEL_QSDDMODELXMLSERIALIZER_H
#include "format/Format.h"
#include "model/parser/SddModelDescriptor.h"

class QSddModelXmlSerializer : public Format<SddModelDescriptor>  {
public:
    QSddModelXmlSerializer();
    QByteArray write(SddModelDescriptor &&object);
    SddModelDescriptor read(const QByteArray &data);

private:
    QJsonObject stateSerialize(const SddModel::State &state);
    SddModel::State stateDeserialize(const QString &str);
    QJsonObject inputSerialize(const SddModel::Input &input);
    SddModel::Input inputDeserialize(const QString &str);
    QJsonObject parametersDeserialize(const SddModel::Parameters &parameters);

};
#endif //TESTSIMULINKMODEL_QSDDMODELJSONSERIALIZER_H
