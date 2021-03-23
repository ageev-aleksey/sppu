#include "gui/camera/QImageView.h"
#include <opencv2/imgproc.hpp>
#include <QMouseEvent>
#include <QHBoxLayout>
QImageView::QImageView(int width, int height, QWidget *parent)
        : QWidget(parent)
{
    //setLayout(new QHBoxLayout);
    m_imgView->setMinimumHeight(1);
    m_imgView->setMinimumWidth(1);
   // m_imgView->setFixedWidth(width);
   // m_imgView->setFixedHeight(height);
   auto l = new QHBoxLayout;
   setLayout(l);
   l->addWidget(m_imgView);
    QWidget::setMouseTracking(true);
}


void QImageView::mouseMoveEvent(QMouseEvent *ev)  {
    emit mouseMove(ev->pos().x(), ev->pos().y());
}


void QImageView::updateImage(cv::Mat img) {
    //       std::cout << "print" << std::endl;
    cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
    QImage image((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
    int w = m_imgView->width();
    int h = m_imgView->height();
    m_imgView->setPixmap(QPixmap::fromImage(image).scaled(w, h, Qt::KeepAspectRatio));
}

void QImageView::setSizeWindow(int width, int height) {
    m_imgView->setFixedWidth(width);
    m_imgView->setFixedHeight(height);
}

void QImageView::closeEvent(QCloseEvent *event) {
    QWidget::closeEvent(event);
}
