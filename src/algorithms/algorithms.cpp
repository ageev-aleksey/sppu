#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "algorithms/algorithms.h"

// 27.04.21
//#define GAUSS_KERNEL_SIZE cv::Size{15, 15}
//#define GAUSS_SIGMA 0
//#define WINDOW_SIZE cv::Size{30, 30}
//#define STEP 100
//#define RED_COLOR {0, 0, 255};
//#define BLUE_COLOR {255, 0, 0};
//#define CIRCLE_RADIUS 10
//#define VIEW_SIZE cv::Size{1280, 720}
//#define ESC_KEY 27

#define GAUSS_KERNEL_SIZE cv::Size{15, 15}
#define GAUSS_SIGMA 0
#define STEP 30
#define WINDOW_SIZE cv::Size{STEP, STEP}
#define RED_COLOR {0, 0, 255};
#define BLUE_COLOR {255, 0, 0};
#define CIRCLE_RADIUS 10
#define VIEW_SIZE cv::Size{1280, 720}
#define ESC_KEY 27

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

cv::Point findRedPointCoordinates(cv::Mat &original)  {
    cv::Mat img;
    cv::cvtColor(original, img, cv::COLOR_BGR2GRAY);
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
            cv::Mat win;
           // win = img(cv::Rect(x, y, xEnd - x, yEnd - y));
          //  std::array<cv::Mat, 3> channels;
          //  cv::split(win, channels);
          //  cv::Mat red;
          //  cv::cvtColor(win, red, cv::COLOR_BGR2GRAY);

            cv::Scalar mu;
            cv::Scalar sigma;
            //cv::meanStdDev(win, mu, sigma);

         //   cv::Mat rtable = zeros(256);
          //  cv::Mat ident = identity(256);
            //for (size_t i = std::ceil(mu.val[0] + 3.0 * sigma.val[0]); i < rtable.rows; i++) {
            //    rtable.at<uchar>(i) = i;
            //}
           /* cv::Mat table;
            cv::merge(std::array<cv::Mat, 3>{rtable}, table);*/
            //cv::LUT(win, rtable, win);
        }
    }

 /*   cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);
    for (size_t i = 0; i < img.rows; i++) {
        for (size_t j = 0; j < img.cols; j++) {
            auto pixel = hsv.at<cv::Vec3b>(i, j);
            auto &h = pixel[0];
            auto &s = pixel[1];
            auto &v = pixel[2];
            if (h > 45) {
                v = 0;
            }
            if (v < 100) {
                v = 0;
            }
            hsv.at<cv::Vec3b>(i, j) = pixel;
        }
    }

    std::array<cv::Mat, 3> hsv_channels;
    cv::split(hsv, hsv_channels);
    cv::Mat &value_channel = hsv_channels[2];*/
    cv::Mat binary;
    cv::threshold(img, binary, 100, 255, cv::THRESH_BINARY);

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
   return centroid;
}