//
// Created by nrx on 29.01.2021.
//

#ifndef SDDCLIENT_QSDDJSONSERIALIZER_H
#define SDDCLIENT_QSDDJSONSERIALIZER_H
#include <QtCore>
#include "gui/model_saver/SavingData.h"
#include "format/Format.h"

namespace sdd::conn {
    class QSddJsonSerializer : public Format<SavingData> {
    public:
        QSddJsonSerializer();
        QByteArray begin() override;
        QByteArray end() override;
        QByteArray next() override;
        QByteArray write(const SavingData &object) override;
        SavingData read(const QByteArray &data) override;
    };
} // namespace sdd::conn


#endif //SDDCLIENT_QSDDJSONSERIALIZER_H
