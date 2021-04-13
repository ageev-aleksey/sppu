//
// Created by nrx on 29.01.2021.
//

#ifndef SDDCLIENT_QSDDJSONSERIALIZER_H
#define SDDCLIENT_QSDDJSONSERIALIZER_H
#include <QtCore>
#include "gui/model_saver/SavingData.h"
#include "format/Format.h"

namespace sdd::conn {
    /**
     * Сериализация информации об устройстве в Json формат
     */
    class QSddJsonSerializer : public Format<SavingData> {
    public:
        QSddJsonSerializer();
        /// Фернуть начало JSON массива
        QByteArray begin() override;
        /// Вернуть Конец JSON массива
        QByteArray end() override;
        /// Вернуть разделитель JSON массива между объектами
        QByteArray next() override;
        /// Преообразовать структуру в JSON объект
        QByteArray write(const SavingData &object) override;
        /// Преобразовать JSON объект в структуру
        SavingData read(const QByteArray &data) override;
    };
} // namespace sdd::conn


#endif //SDDCLIENT_QSDDJSONSERIALIZER_H
