//
// Created by nrx on 07.03.2021.
//

#ifndef SDDCLIENT_MODELDATAGETTER_H
#define SDDCLIENT_MODELDATAGETTER_H

#include <QtCore>
#include "gui/model_saver/SavingData.h"


/**
 * Интерфейс для передачи данных от источника потребителям
 * через цикл событий Qt
 */
class QModelDataGetter : public QObject{
    Q_OBJECT
public:
    virtual ~QModelDataGetter() = default;
signals:
    /// Отправить данные в Цикл событий потребителям
    void newData(SavingData &data);
};
#endif //SDDCLIENT_MODELDATAGETTER_H
