#include "camera/QRtspCamera.h"
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <stdexcept>
#include <atomic>
#include <QtGui/QImage>

#define GAUSS_KERNEL_SIZE cv::Size{15, 15}
#define GAUSS_SIGMA 0
#define WINDOW_SIZE cv::Size{100, 100}
#define STEP 100
#define RED_COLOR {0, 0, 255};
#define BLUE_COLOR {255, 0, 0};
#define CIRCLE_RADIUS 10
#define VIEW_SIZE cv::Size{1280, 720}
#define ESC_KEY 27

namespace util {
    cv::Mat zeros(size_t n) {
        cv::Mat ret(256, 1, CV_8UC1);
        ret.resize(n);
        for (size_t i = 0; i < n; i++) {
            ret.at<uchar>(i) = 0;
        }
        return ret;
    }

    cv::Mat identity(size_t n) {
        cv::Mat ret(256, 1, CV_8UC1);
        ret.resize(n);
        for (size_t i = 0; i < n; i++) {
            ret.at<uchar>(i) = i;
        }
        return ret;
    }
}

void  processingImage(cv::Mat& original) {
    cv::Mat img = original;
    cv::Mat hsv;
    cv::Point centroid;
    cv::Scalar color = BLUE_COLOR;
    _sleep(2000);
    cv::medianBlur(img, img, 5);
    cv::GaussianBlur(img, img, GAUSS_KERNEL_SIZE, GAUSS_SIGMA);

    for (size_t x = 0; x < img.cols; x += STEP) {
        for (size_t y = 0; y < img.rows; y += STEP) {
            size_t xEnd = x + STEP;
            size_t yEnd = y + STEP;
            if (xEnd >= img.cols) {
                xEnd = img.cols - 1;
            }
            if (yEnd >= img.rows) {
                yEnd = img.rows - 1;
            }
            cv::Mat win = img(cv::Rect(x, y, xEnd - x, yEnd - y));
            std::array<cv::Mat, 3> channels;
            cv::split(win, channels);
            cv::Mat& red = channels[2];

            cv::Scalar mu;
            cv::Scalar sigma;
            cv::meanStdDev(red, mu, sigma);

            cv::Mat rtable = util::zeros(256);
            cv::Mat identity = util::identity(256);
            for (size_t i = std::ceil(mu.val[0] + 3.0 * sigma.val[0]); i < rtable.rows; i++) {
                rtable.at<uchar>(i) = i;
            }
            cv::Mat table;
            cv::merge(std::array<cv::Mat, 3>{identity, identity, rtable}, table);
            cv::LUT(win, table, win);
        }
    }

    cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);
    for (size_t i = 0; i < img.rows; i++) {
        for (size_t j = 0; j < img.cols; j++) {
            auto pixel = hsv.at<cv::Vec3b>(i, j);
            auto& h = pixel[0];
            auto& s = pixel[1];
            auto& v = pixel[2];
            if (h > 15) {
                v = 0;
            }
            hsv.at<cv::Vec3b>(i, j) = pixel;
        }
    }

    std::array<cv::Mat, 3> hsv_channels;
    cv::split(hsv, hsv_channels);
    cv::Mat& value_channel = hsv_channels[2];
    cv::Mat binary;
    cv::threshold(value_channel, binary, 10, 255, cv::THRESH_BINARY);

    cv::Moments m = cv::moments(binary, true);
    if (m.m00 != 0) {
        centroid.x = m.m10 / m.m00;
        centroid.y = m.m01 / m.m00;
        color = RED_COLOR;
    }
    else {
        color = BLUE_COLOR;
    }

    cv::circle(original, centroid, CIRCLE_RADIUS, color, cv::FILLED);
}




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
        cv::Mat frame;
        capture >> frame;
        if (!frame.empty()) {
           // cv::imshow("test", frame);
            //QImage img((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB32);
          //  QImage img = Mat2QImage(frame);
            // cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
            // QImage image((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
           
           processingImage(frame);
            cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
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

QRtspCamera::QRtspCamera(const std::string rtspUrl) {
    qRegisterMetaType<cv::Mat>("cv::Mat");
    auto worker = new QRtspCameraWorker(rtspUrl);
   // worker->capture.open(rtspUrl);
    if (!worker->capture.isOpened()) {
        throw std::runtime_error("Error connection source");
    }
    m_pWorkerThread = new QThread(this);
    QObject::connect(m_pWorkerThread, &QThread::started, worker, &QRtspCameraWorker::capturingInit);
    QObject::connect(worker, &QRtspCameraWorker::recvImg, this, &QRtspCamera::recvImage);
    worker->moveToThread(m_pWorkerThread);
    m_pWorkerThread->start();
}

void QRtspCamera::play() {

}

void QRtspCamera::stop() {

}

class RtspCameraFactory : public QICameraFactory {
public:
    std::unique_ptr<QICamera> create(const std::string &path) override {
        return std::make_unique<QRtspCamera>(path);
    }
};

std::unique_ptr<QICameraFactory> QCameraMaker::make() {
    return std::make_unique<RtspCameraFactory>();
}


#include "QRtspCamera.moc"