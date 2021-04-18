//
// Created by nrx on 07.03.2021.
//

#ifndef SDDCLIENT_SAVINGDATA_H
#define SDDCLIENT_SAVINGDATA_H

#include "sdd_protocol/connect/QIConnection.h"
#include <unordered_map>

/**
 * Информация об состоянии устройства с дополнительными полями
 */
struct SavingData {
    /// Информация об состоянии устройства
    sdd::conn::State state;
    /// Дополнительные не обязательные поля
    std::unordered_map<std::string, std::string> additional;
};
#endif //SDDCLIENT_SAVINGDATA_H
