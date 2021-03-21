//
// Created by nrx on 07.02.2021.
//

#ifndef SDDCLIENT_QRTSPCAMERA_H
#define SDDCLIENT_QRTSPCAMERA_H

#include "camera/QICamera.h"

class QRtspCamera : public QICamera {
    Q_OBJECT
public:
    explicit QRtspCamera(const std::string rstpUrl);
public slots:
    void play() override;
    void stop() override;

private:
    QThread *m_pWorkerThread;
};

#endif //SDDCLIENT_QRTSPCAMERA_H
