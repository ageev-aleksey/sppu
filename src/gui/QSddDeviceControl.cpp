#include "gui/QSddDeviceControl.h"


QSddDeviceControl::QSddDeviceControl(std::unique_ptr<sdd::conn::IConnection> connection)
    : m_pConnection(std::move(connection))
{

}

std::vector<sdd::conn::State> QSddDeviceControl::getSddStates() {
    return std::vector<sdd::conn::State>();
}

void QSddDeviceControl::guiInit() {

}
