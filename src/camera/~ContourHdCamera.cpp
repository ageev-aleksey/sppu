//
// Created by nrx on 28.02.2021.
//

//#include "camera/~ContourHdCamera.h"
//
//
//class QContourHdCameraWorker : public QObject {
//Q_OBJECT
//public:
//    QContourHdCameraWorker(std::string url)
//            : capture(url)
//    {}
//
//    void start() const {
//        timer->start();
//    }
//
//    void stop() const {
//        timer->stop();
//    }
//public slots:
//    void capturingInit() {
//        timer = new QTimer(this);
//        timer->setInterval(0);
//        QObject::connect(timer, &QTimer::timeout, this, &QContourHdCameraWorker::imageCapture);
//       // timer->start();
//    }
//
//
//
//    void imageCapture()  {
//        cv::Mat frame;
//        capture >> frame;
//        if (!frame.empty()) {
//            cv::imshow("test", frame);
//            //QImage img((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB32);
//            //  QImage img = Mat2QImage(frame);
//            cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
//            emit recvImg(frame);
//        }
//
//    }
//
//
//signals:
//    void recvImg(const cv::Mat img);
//
//public:
//    cv::VideoCapture capture;
//    std::atomic<bool> isRun = true;
//    QTimer *timer{};
//};
//
//
//
//void QContourHdCamera::play() {
//
//}
//
//void QContourHdCamera::stop() {
//
//}
