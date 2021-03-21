//
// Created by nrx on 08.03.2021.
//

#ifndef SDDCLIENT_QIMAGEVIEW_H
#define SDDCLIENT_QIMAGEVIEW_H

#include <QtCore>
#include <QWidget>
#include <QLabel>
#include <opencv2/core/mat.hpp>

class QImageView : public QWidget {
    Q_OBJECT
public:
    QImageView(int width, int height, QWidget *parent = nullptr);

    void mouseMoveEvent(QMouseEvent *ev) override;
public slots:
    void updateImage(cv::Mat img);
    void setSizeWindow(int width, int height);
signals:
    void mouseMove(int x, int y);
private:
    QLabel *m_imgView = new QLabel(this);
};

#endif //SDDCLIENT_QIMAGEVIEW_H
