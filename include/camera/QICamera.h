//
// Created by nrx on 07.02.2021.
//

#ifndef SDDCLIENT_QICAMERA_H
#define SDDCLIENT_QICAMERA_H

#include <QtCore>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <memory>

/**
 * Интерфейс доступа к камере
 */
class QICamera : public QObject {
    Q_OBJECT
public slots:
    /// Запуск передачи кадров
    virtual void play() = 0;
    /// Отсановка передачи кадров
    virtual void stop() = 0;
signals:
    /// Сигнал - получен новый кадр от камеры
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



#endif //SDDCLIENT_QICAMERA_H
