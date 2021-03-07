//
// Created by nrx on 07.03.2021.
//

#ifndef SDDCLIENT_MODELDATAGETTER_H
#define SDDCLIENT_MODELDATAGETTER_H

#include <QtCore>
#include "gui/model_saver/SavingData.h"


class QModelDataGetter : public QObject{
    Q_OBJECT
public:
    virtual ~QModelDataGetter() = default;
signals:
    void newData(SavingData &data);
};
#endif //SDDCLIENT_MODELDATAGETTER_H
