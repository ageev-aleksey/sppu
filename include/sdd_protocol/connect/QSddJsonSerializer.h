//
// Created by nrx on 29.01.2021.
//

#ifndef SDDCLIENT_QSDDJSONSERIALIZER_H
#define SDDCLIENT_QSDDJSONSERIALIZER_H
#include <QtCore>
#include "sdd_protocol/connect/States.h"
#include "format/Format.h"

namespace sdd::conn {
    class QSddJsonSerializer : public Format<State> {
    public:
        QSddJsonSerializer();
        QByteArray begin() override;
        QByteArray end() override;
        QByteArray next() override;
        QByteArray write(const State &object) override;
        State read(const QByteArray &data) override;
    };
} // namespace sdd::conn


#endif //SDDCLIENT_QSDDJSONSERIALIZER_H
