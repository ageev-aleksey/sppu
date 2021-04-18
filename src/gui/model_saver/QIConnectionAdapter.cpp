#include "gui/model_saver/QIConnectionAdapter.h"
#include <assert.h>

QIConnectionAdapter::QIConnectionAdapter(std::shared_ptr<sdd::conn::QIConnection> connection) {
    assert(connection != nullptr);
    mConnection = connection;
    QObject::connect(mConnection.get(), &sdd::conn::QIConnection::recvStatePackage,
                     this, &QIConnectionAdapter::dataRecv);
}

void QIConnectionAdapter::dataRecv(const sdd::conn::State &state) {
    SavingData data;
    data.state = state;
    emit QModelDataGetter::newData(data);
}
