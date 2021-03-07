//
// Created by nrx on 07.03.2021.
//

#ifndef SDDCLIENT_QCAMERA_H
#define SDDCLIENT_QCAMERA_H

#include "camera/QICamera.h"

#include <QtCore>
#include <QWidget>

#include <memory>
#include <camera/QICamera.h>


class QCameraWindow : public QWidget {
    Q_OBJECT
public:
    QCameraWindow();
private:

    std::shared_ptr<QICamera> camera;

};
#endif //SDDCLIENT_QCAMERA_H
