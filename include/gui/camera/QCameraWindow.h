//
// Created by nrx on 07.03.2021.
//

#ifndef SDDCLIENT_QCAMERA_H
#define SDDCLIENT_QCAMERA_H

#include "camera/QICamera.h"

#include <QtCore>
#include <QWidget>
#include <QComboBox>
#include <QLayout>

#include "gui/camera/QImageView.h"

#include <memory>
#include <camera/QICamera.h>

class QCameraWindowDialogContentImpl;
class QCameraWindow : public QWidget {
    Q_OBJECT
public:
    explicit QCameraWindow(QWidget *parnet=nullptr);
    ~QCameraWindow() override;
private:
    void initDialog();
    void initImageView();
    std::shared_ptr<QICamera> camera;
private slots:
    void comboBoxUpdate(const QString &name);
    void cameraConnection();
private:

    // Окно подключения к камере
    QComboBox *mConnectionsType = new QComboBox(this);
    QLayout *mDialogLayout = new QHBoxLayout(this);
    QCameraWindowDialogContentImpl *mContent;
    // Окно получение данных с камерых
    std::unique_ptr<QImageView> mImageView = nullptr;
    //Камера
    std::unique_ptr<QICamera> mCamera = nullptr;
};
#endif //SDDCLIENT_QCAMERA_H
