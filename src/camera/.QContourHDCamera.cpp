#pragma comment (lib,"Ws2_32.lib")

#include "camera/QContourHDCamera.h"
#include "camera/CountourHdDriver.h"
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <stdexcept>
#include <atomic>
#include <QtGui/QImage>
#include <fstream>
#include <jpeglib.h>


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

char jpg_name[] = "C:\\my_tmp\\tmp.jpg";
static cv::Mat glob_img;
void contourHd_callback(void* param,
               const void* jpeg_img, unsigned int jpeg_size, int reason)
{
   // cv::Mat raw(1, jpeg_size, CV_8UC1, jpeg_img);

   // Если callback-функция была вызвана для оповещения об ошибке,
   // либо уже сохранено 10 кадров, то выход.
  if ((reason != VKVR_OK)) return;
  //_sleep(2000);
  //jpg_name[7] = '0' + (char)jpeg_saved;
  // Создание файла JPEG.
  HANDLE hf = CreateFile(jpg_name,
      GENERIC_READ | GENERIC_WRITE,
      (DWORD)0,
      NULL,
      CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      (HANDLE)NULL);
  if (hf == INVALID_HANDLE_VALUE)
  {
      fprintf(stderr, "Failed to create file: \"%s\".\n", jpg_name);
      return;
  };

  // Запись данных в файл JPEG.
   DWORD dwTmp;
  if (!WriteFile(hf, jpeg_img, jpeg_size, (LPDWORD)&dwTmp, NULL))
      fprintf(stderr, "Failed to write to file: \"%s\".\n", jpg_name);
  else
      fprintf(stdout, "Captured image successfully saved to \"%s\".\n", jpg_name);

  // Закрытие файла JPEG.
  CloseHandle(hf);

  glob_img = cv::imread(jpg_name);

  processingImage(glob_img);
  cv::imshow("img", glob_img);
  //cv::waitKey(0);
  cv::cvtColor(glob_img, glob_img, cv::COLOR_BGR2RGB);

  /*std::ofstream stream;
  stream.open("C:\\my_tmp\\tmp.jpg");
  if (!stream.is_open()) {
      std::cout << "ERRor opening" << std::endl;
  }
  stream.write((const char*)jpeg_img, jpeg_size);
  stream.close();
  std::cout << "Test" << std::endl;*/

 /* struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;

  jpeg_mem_src(&cinfo, (const unsigned char*)jpeg_img, jpeg_size);

  std::cout << "Error" << std::endl;

  switch (jpeg_read_header(&cinfo, TRUE)) {
  case JPEG_SUSPENDED:
  case JPEG_HEADER_TABLES_ONLY:
      return;
  case JPEG_HEADER_OK:
      break;
  }

  cinfo.out_color_space = JCS_EXT_BGR;

  jpeg_start_decompress(&cinfo);

  glob_img = cv::Mat(
      cv::Size(cinfo.output_width, cinfo.output_height),
      CV_8UC3);

  while (cinfo.output_scanline < cinfo.output_height) {
      JSAMPLE* row = glob_img.ptr(cinfo.output_scanline);
      jpeg_read_scanlines(&cinfo, &row, 1);
  }

  jpeg_finish_decompress(&cinfo);*/

};



class QContourHdCameraWorker : public QObject {
    Q_OBJECT
public:
    QContourHdCameraWorker()
    {}
public slots:
    void capturingInit() {
        vkvrcap = new VKVRCapture();

        // Инициализация объекта захвата кадров.
      if (vkvrcap->init() == VKVR_OK)
         // Запуск захвата кадров.
         if (vkvrcap->play(contourHd_callback, vkvrcap,
                           1, "192.168.1.250", AUTH_ROOT0000) == VKVR_OK)
         {
            // 5 секунд ожидание захвата кадров.
            /*for (int i = 0; (i < 5) && (jpeg_saved < 10); ++i)
               Sleep(1000);
            if (!jpeg_saved)
               fprintf(stderr, "Failed to capture the image.\n");*/


            timer = new QTimer(this);
            timer->setInterval(0);
            QObject::connect(timer, &QTimer::timeout, this, &QContourHdCameraWorker::imageCapture);
            timer->start();
         }
         else
            fprintf(stderr, "Failed to start the capture.\n");
      else
         fprintf(stderr, "Failed to initialize object VKVRCapture.\n");




    }
    void imageCapture()  {
       bool status =  vkvrcap->capture_work_routine();
       if (status == true) {
           cv::Mat img = glob_img.clone();
           emit recvImg(img);
          // cv::Mat img = cv::imread(jpeg);
       }
       // cv::Mat frame;
       // capture >> frame;
      //  if (!frame.empty()) {
           // cv::imshow("test", frame);
            //QImage img((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB32);
          //  QImage img = Mat2QImage(frame);
            // cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
            // QImage image((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
           // emit recvImg(frame);
      //  }

    }
signals:
    void recvImg(cv::Mat img);

public:
    std::atomic<bool> isRun = true;
    QTimer *timer{};

    VKVRCapture* vkvrcap = nullptr;
};

QContourHdCamera::QContourHdCamera() {
    qRegisterMetaType<cv::Mat>("cv::Mat");
    auto worker = new QContourHdCameraWorker;
   // worker->capture.open(rtspUrl);
   /* if (!worker->capture.isOpened()) {
        throw std::runtime_error("Error connection source");
    }*/
    m_pWorkerThread = new QThread(this);
    QObject::connect(m_pWorkerThread, &QThread::started, worker, &QContourHdCameraWorker::capturingInit);
    QObject::connect(worker, &QContourHdCameraWorker::recvImg, this, &QContourHdCamera::recvImage);
    worker->moveToThread(m_pWorkerThread);
    m_pWorkerThread->start();
}

void QContourHdCamera::play() {

}

void QContourHdCamera::stop() {

}


#include "QContourHDCamera.moc"