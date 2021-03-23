//
// Created by nrx on 07.03.2021.
//

#ifndef SDDCLIENT_QICONNECTIONWRAPPER_H
#define SDDCLIENT_QICONNECTIONWRAPPER_H

#include "gui/model_saver/QModelDataGetter.h"
#include "sdd_protocol/connect/QIConnection.h"
#include <memory>

/**
 * Адаптер
 * Адаптирование интерфейса подключения к устройству
 * к интерфейсу источника данных
 */
class QIConnectionAdapter : public QModelDataGetter  {
public:
    explicit QIConnectionAdapter(std::shared_ptr<sdd::conn::QIConnection> connection);
private slots:
    void dataRecv(const sdd::conn::State &state);
private:
    std::shared_ptr<sdd::conn::QIConnection> mConnection;
    QMetaObject::Connection mEventConnection;
};
#endif //SDDCLIENT_QICONNECTIONWRAPPER_H
