//
// Created by nrx on 07.02.2021.
//

#ifndef SDDCLIENT_QRTSPCAMERA_H
#define SDDCLIENT_QRTSPCAMERA_H

#include "camera/QICamera.h"

class QRtspCameraWorker;
class QRtspCamera : public QICamera {
    Q_OBJECT
public:
    explicit QRtspCamera(const std::string uri);
    ~QRtspCamera() override;
public slots:
    void play() override;
    void stop() override;

private:
    QThread *m_pWorkerThread;
    QRtspCameraWorker *m_worker;
    std::string m_uri;
};

#endif //SDDCLIENT_QRTSPCAMERA_H
