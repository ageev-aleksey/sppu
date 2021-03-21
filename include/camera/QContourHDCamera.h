#ifndef SDDCLIENT_QCONTOURHDCAMERA_H
#define SDDCLIENT_QCONTOURHDCAMERA_H

#include "camera/QICamera.h"

class QContourHdCamera : public QICamera {
    Q_OBJECT
public:
    explicit QContourHdCamera();
public slots:
    void play() override;
    void stop() override;

private:
    QThread *m_pWorkerThread;
};

#endif //SDDCLIENT_QCONTOURHDCAMERA_H