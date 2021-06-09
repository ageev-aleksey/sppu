//#include <iostream>
//
//
//int *make_matrix(int row, int col) {
//    int *m = new int[row*col];
//    int v = 1;
//    for (int i = 0; i < row; i++) {
//        for (int j = 0; j < col; j++) {
//            m[i*col + j] = v;
//            v++;
//        }
//    }
//    return m;
//}
//
//void circle_walk(int *matrix, int row, int col) {
//    int dr = -1; // шаг перемещения по строке
//    int dc = 0; // шаг перемещения по столбцу
//    int len = 1; // количество эелементов до переключения направления
//    int r = row/2;
//    int c = col/2;
//
//    while (true) {
//        if (r >= row || r < 0 || c >= col || c < 0) {
//            break;
//        }
//        for (int i = 0; i < len; i++) {
//            std::cout << matrix[r*col+c] << std::endl;
//            r += dr;
//            c += dc;
//        }
//        if (dr == 0) {
//            dr = dc;
//            dc = 0;
//            len++;
//        } else {
//            dc = -dr;
//            dr = 0;
//        }
//    }
//}
//
//void circle_full_walk(int *matrix, int row, int col) {
//    int dr = -1; // шаг перемещения по строке
//    int dc = 0;  // шаг перемещения по столбцу
//    int len = 1; // количество эелементов до переключения направления
//    int r = 0;//row/2;
//    int c = 2;//col/2;
//    int s = 1;
//    std::cout << matrix[r*col+c] << std::endl;
//    while (s != row*col) {
//        for (int i = 0; i < len; i++) {
//            r += dr;
//            c += dc;
//            if ((r < 0 || r >= row) || (c < 0 || c >= col)) {
//                r += (len-i-1)*dr;
//                c += (len-i-1)*dc;
//                break;
//            } else {
//                std::cout << matrix[r*col+c] << std::endl;
//                s++;
//            }
//        }
//        if (dr == 0) {
//            dr = dc;
//            dc = 0;
//            len++;
//        } else {
//            dc = -dr;
//            dr = 0;
//        }
//    }
//}
//
//
//int main () {
//    auto matrix = make_matrix(4, 4);
//    circle_full_walk(matrix, 4, 4);
//    delete [] matrix;
//}




/////////////////////////////////////////////////
//#include "Shot.h"
//#include <iostream>
//#include <QtCore>
//#include <QNetworkAccessManager>
//#include <QNetworkReply>
//



#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>



#define GAUSS_KERNEL_SIZE cv::Size{15, 15}
#define GAUSS_SIGMA 0
#define WINDOW_SIZE cv::Size{20, 20}
#define STEP 100
#define RED_COLOR {0, 0, 255};
#define BLUE_COLOR {255, 0, 0};
#define CIRCLE_RADIUS 10
#define VIEW_SIZE cv::Size{1280, 720}
#define ESC_KEY 27

namespace {
    cv::Mat zeros(size_t n) {
        cv::Mat ret(256, 1, CV_8UC1);
        ret.resize(n);
        for (size_t i = 0; i < n; i++) {
            ret.at<uchar>(i) = 0;
        }
        return ret;
    }

    cv::Mat identity_(size_t n) {
        cv::Mat ret(256, 1, CV_8UC1);
        ret.resize(n);
        for (size_t i = 0; i < n; i++) {
            ret.at<uchar>(i) = i;
        }
        return ret;
    }


    void processingImage(cv::Mat &original) {
        cv::Mat img = original;
        cv::Mat hsv;
        cv::Point centroid;
        cv::Scalar color = BLUE_COLOR;
        //_sleep(2000);
        // cv::medianBlur(img, img, 5);
        // cv::GaussianBlur(img, img, GAUSS_KERNEL_SIZE, GAUSS_SIGMA);

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
                cv::Mat &red = channels[2];

                cv::Scalar mu;
                cv::Scalar sigma;
                cv::meanStdDev(red, mu, sigma);

                cv::Mat rtable = zeros(256);
                cv::Mat identity = identity_(256);
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
                auto &h = pixel[0];
                auto &s = pixel[1];
                auto &v = pixel[2];
                if (h > 15) {
                    v = 0;
                }
                hsv.at<cv::Vec3b>(i, j) = pixel;
            }
        }

        std::array<cv::Mat, 3> hsv_channels;
        cv::split(hsv, hsv_channels);
        cv::Mat &value_channel = hsv_channels[2];
        cv::Mat binary;
        cv::threshold(value_channel, binary, 10, 255, cv::THRESH_BINARY);

        cv::Moments m = cv::moments(binary, true);
        if (m.m00 != 0) {
            centroid.x = m.m10 / m.m00;
            centroid.y = m.m01 / m.m00;
            color = RED_COLOR;
        } else {
            color = BLUE_COLOR;
        }

        cv::circle(original, centroid, CIRCLE_RADIUS, color, cv::FILLED);
        cv::imshow("original", original);
        cv::imshow("bin", binary);
    }

}



int main(int argc, char **argv) {
//    QCoreApplication a(argc, argv);
//    Shot *s = new Shot(&a);
//    return a.exec();
    cv::VideoCapture cap;
    cap.open("rtsp://192.168.43.1:5554/camera");
    if (!cap.isOpened()) {
        std::cout << "Файл не открылся" << std::endl;
        return -1;
    }
    cv::namedWindow( "myVideo", cv::WINDOW_AUTOSIZE );
    cv::Mat frame;
    while(true) {
        cap >> frame;
        if( frame.empty() ){
            std::cout << "Could not load the video frames. \n";
            break;
        }
        cv::imshow( "myVideo", frame );
        processingImage(frame);
        if( cv::waitKey(1) >= 0 ){
            std::cout << "Escape pressed \n";
            break;
        }
    }
    return 0;
}