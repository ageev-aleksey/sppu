#include "sdd_protocol/connect/IConnection.h"



//bool sdd::conn::IConnection::callbackIsContain(const std::function<Handle> &handler) {
//    auto itr = std::find(m_callbacks.begin(), m_callbacks.end(), handler);
//    return itr != m_callbacks.end();
//}


void sdd::conn::IConnection::addCallbackDataReady(const std::function<Handle> &handler) {
    if (!handler) {
        throw std::runtime_error("invalid handler");
    }
    std::lock_guard lock(m_callbackMutex);
    m_callbacks.push_back(handler);
}

//void sdd::conn::IConnection::removeCallback(const std::function<Handle> &handler) {
//    std::lock_guard lock(m_callbackMutex);
//    auto itr = std::find(m_callbacks.begin(), m_callbacks.end(), handler);
//    if (itr != m_callbacks.end()) {
//        m_callbacks.erase(itr);
//    }
//}

void sdd::conn::IConnection::allCall(const sdd::conn::State &state) {
    std::vector<std::function<Handle>> handlers;
    {
        std::lock_guard callbackLock(m_callbackMutex);
        if (m_callbacks.empty()) {
            return;
        }
        handlers = m_callbacks;
    }
    for (auto &callback : handlers) {
        callback(state);
    }
}
