//
// Created by nrx on 07.02.2021.
//

#ifndef SDDCLIENT_QCAMERA_H
#define SDDCLIENT_QCAMERA_H

#include <QtCore>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <memory>

class QICamera : public QObject {
    Q_OBJECT
public slots:
    virtual void play() = 0;
    virtual void stop() = 0;
signals:
    void recvImage(cv::Mat img);
};

class QICameraFactory {
public:
    virtual std::unique_ptr<QICamera> create(const std::string &path) = 0;
};

class QCameraMaker {
public:
    static std::unique_ptr<QICameraFactory> make();
};



#endif //SDDCLIENT_QCAMERA_H
