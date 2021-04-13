//
// Created by nrx on 07.02.2021.
//

#ifndef SDDCLIENT_QCVVCCAMERA_H
#define SDDCLIENT_QCVVCCAMERA_H

#include "camera/QICamera.h"

class QRtspCameraWorker;
/**
 * Камера адаптер для объекта cv::VideoCapture
 */
class QCvVcCamera : public QICamera {
    Q_OBJECT
public:
    explicit QCvVcCamera(const std::string uri);
    ~QCvVcCamera() override;
public slots:
    void play() override;
    void stop() override;

private:
    QThread *m_pWorkerThread;
    QRtspCameraWorker *m_worker;
    std::string m_uri;
};

#endif //SDDCLIENT_QCVVCCAMERA_H
