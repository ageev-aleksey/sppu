//
// Created by nrx on 17.01.2021.
//

#ifndef SDDCLIENT_SDDCONTROLLER_H
#define SDDCLIENT_SDDCONTROLLER_H

#include "sdd_protocol/connect/IConnection.h"
#include <vector>
#include <QWidget>

class QISddStateWidget : public QWidget {
public:
    virtual std::vector<sdd::conn::State> getSddStates() = 0;
};

#endif //SDDCLIENT_SDDCONTROLLER_H
