//
// Created by nrx on 17.01.2021.
//

#ifndef SDDCLIENT_QSDDDEVICECONTROL_H
#define SDDCLIENT_QSDDDEVICECONTROL_H
#include "gui/QISddStateWidget.h"
#include "sdd_protocol/connect/IConnection.h"
#include <QtWidgets>

#include <memory>


class QSddDeviceControl : public QISddStateWidget {
public:
    explicit QSddDeviceControl(std::unique_ptr<sdd::conn::IConnection> connection);
    std::vector<sdd::conn::State> getSddStates() override;
private:
    void guiInit();
    std::unique_ptr<sdd::conn::IConnection> m_pConnection;
};

#endif //SDDCLIENT_QSDDDEVICECONTROL_H
