#include "gui/model_saver/QIConnectionWrapper.h"
#include <assert.h>

QIConnectionWrapper::QIConnectionWrapper(std::shared_ptr<sdd::conn::QIConnection> connection) {
    assert(connection != nullptr);
    mConnection = connection;
    QObject::connect(mConnection.get(), &sdd::conn::QIConnection::recvStatePackage,
                     this, &QIConnectionWrapper::dataRecv);
}

void QIConnectionWrapper::dataRecv(const sdd::conn::State &state) {
    SavingData data;
    data.state = state;
    emit QModelDataGetter::newData(data);
}
