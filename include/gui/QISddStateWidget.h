//
// Created by nrx on 17.01.2021.
//

#ifndef SDDCLIENT_SDDCONTROLLER_H
#define SDDCLIENT_SDDCONTROLLER_H

#include "sdd_protocol/connect/QIConnection.h"
#include <vector>
#include <QWidget>
#include <memory>

/**
 * Интерфейс виджета получения доступа к состоянию Сферчиеского привода
 */
class QISddStateWidget : public QWidget {
public:
    /// Получить последние доступные состояния устройства
    virtual std::vector<sdd::conn::State> getSddStates() = 0;
    /**
     * Загрузить настройки
     * @param settings объект доступа к настройкам
     */
    virtual void settingsLoad(const QSettings &settings) = 0;
    /**
     * Сохранить настройки
     * @param settings объект доступа к настройкам
     */
    virtual void settingsStore(QSettings &settings) = 0;
    /**
     * Получить реализацию интерфейса доступа к устройству
     * @return указатель на объект, реализующий доступ
     */
    virtual std::shared_ptr<sdd::conn::QIConnection> getSddConnection() = 0;
    // addCallbackStateReceived()
    // removeCallbackStateReceived()
};

#endif //SDDCLIENT_SDDCONTROLLER_H
