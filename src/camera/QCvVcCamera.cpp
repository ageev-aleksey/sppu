#include "camera/QCvVcCamera.h"
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <stdexcept>
#include <atomic>
#include <QtGui/QImage>
#include <sstream>


namespace {
    QImage Mat2QImage(const cv::Mat3b &src) {
        QImage dest(src.cols, src.rows, QImage::Format_ARGB32);
        for (int y = 0; y < src.rows; ++y) {
            const cv::Vec3b *srcrow = src[y];
            QRgb *destrow = (QRgb*)dest.scanLine(y);
            for (int x = 0; x < src.cols; ++x) {
                destrow[x] = qRgba(srcrow[x][2], srcrow[x][1], srcrow[x][0], 255);
            }
        }
        return dest;
    }


//    QImage Mat2QImage(const cv::Mat_<double >&src)
//    {
//        double scale = 255.0;
//        QImage dest(src.cols, src.rows, QImage::Format_ARGB32);
//        for (int y = 0; y < src.rows; ++y) {
//            const double *srcrow = src[y];
//            QRgb *destrow = (QRgb*)dest.scanLine(y);
//            for (int x = 0; x < src.cols; ++x) {
//                unsigned int color = srcrow[x] * scale;
//                destrow[x] = qRgba(color, color, color, 255);
//            }
//        }
//        return dest;
//    }
}

class QRtspCameraWorker : public QObject {
    Q_OBJECT
public:
    QRtspCameraWorker(std::string url)
    : capture(url)
    {}
public slots:
    void capturingInit() {
        timer = new QTimer(this);
        timer->setInterval(0);
        QObject::connect(timer, &QTimer::timeout, this, &QRtspCameraWorker::imageCapture);
        timer->start();
    }
    void imageCapture()  {
        if (QThread::currentThread()->isInterruptionRequested()) {
            capture.release();
            timer->stop();
            QThread::currentThread()->quit();
        }
        cv::Mat frame;
        capture >> frame;
        if (!frame.empty()) {
           // cv::imshow("test", frame);
            //QImage img((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB32);
          //  QImage img = Mat2QImage(frame);
            // cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
            // QImage image((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
           
            //processingImage(frame);
            //cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
            emit recvImg(frame);
        }

    }
signals:
    void recvImg(cv::Mat img);

public:
    cv::VideoCapture capture;
    std::atomic<bool> isRun = true;
    QTimer *timer{};
};

QCvVcCamera::QCvVcCamera(const std::string uri) {
    m_uri = std::move(uri);
    qRegisterMetaType<cv::Mat>("cv::Mat");
    m_worker = new QRtspCameraWorker(m_uri);
    if (!m_worker->capture.isOpened()) {
        throw std::runtime_error("Error connection source");
    }
    m_pWorkerThread = new QThread(this);
    QObject::connect(m_pWorkerThread, &QThread::started, m_worker, &QRtspCameraWorker::capturingInit);
    QObject::connect(m_worker, &QRtspCameraWorker::recvImg, this, &QCvVcCamera::recvImage);
    m_worker->moveToThread(m_pWorkerThread);
}

void QCvVcCamera::play() {
    if (!m_pWorkerThread->isRunning()) {
        if (!m_worker->capture.isOpened()) {
            if (!m_worker->capture.open(m_uri)) {
                std::stringstream  msg;
                msg << "Error opening video source by uri: " << m_uri;
                throw std::runtime_error(msg.str());
            }
        }
        m_pWorkerThread->start();
    }
}

void QCvVcCamera::stop() {
    if (m_pWorkerThread->isRunning()) {
        m_pWorkerThread->requestInterruption();
        m_pWorkerThread->wait();
    }
}

QCvVcCamera::~QCvVcCamera() {
    delete m_pWorkerThread;
}

class RtspCameraFactory : public QICameraFactory {
public:
    std::unique_ptr<QICamera> create(const std::string &path) override {
        return std::make_unique<QCvVcCamera>(path);
    }
};

std::unique_ptr<QICameraFactory> QCameraMaker::make() {
    return std::make_unique<RtspCameraFactory>();
}


#include "QCvVcCamera.moc"