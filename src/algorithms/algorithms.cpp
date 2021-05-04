#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
//#include "algorithms/algorithms.h"
#include <map>

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

#define GAUSS_KERNEL_SIZE cv::Size{3, 3}
#define GAUSS_SIGMA 0
#define STEP 40
#define WINDOW_SIZE cv::Size{STEP, STEP}
#define RED_COLOR {0, 0, 255}
#define BLUE_COLOR {255, 0, 0}
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


std::map<int, int>  binarySegmentation(cv::Mat binary);

void binaryPrint(cv::Mat binary) {
    for (size_t i = 0; i < binary.rows; i++) {
        for (size_t j = 0; j < binary.cols; j++) {
            std::cout << (uint)binary.at<uchar>(i, j) << " ";
        }
        std::cout << std::endl;
    }
}

cv::Point findRedPointCoordinates(cv::Mat &original)  {
    cv::Mat img;
    cv::GaussianBlur(original, original, GAUSS_KERNEL_SIZE, GAUSS_SIGMA);
    cv::cvtColor(original, img, cv::COLOR_BGR2GRAY);

    cv::Mat hsv;
    cv::Point centroid;
    cv::Scalar color = BLUE_COLOR;
    //_sleep(2000);
    // cv::medianBlur(img, img, 5);
   // cv::GaussianBlur(img, img, GAUSS_KERNEL_SIZE, GAUSS_SIGMA);
    cv::imshow("gray", img);

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
            win = img(cv::Rect(x, y, xEnd - x, yEnd - y));
          //  std::array<cv::Mat, 3> channels;
          //  cv::split(win, channels);
          //  cv::Mat red;
          //  cv::cvtColor(win, red, cv::COLOR_BGR2GRAY);

            cv::Scalar mu;
            cv::Scalar sigma;
            cv::meanStdDev(win, mu, sigma);

              cv::Mat rtable = zeros(256);
          //  cv::Mat ident = identity(256);
            for (size_t i = std::ceil(mu.val[0] + 4.0 * sigma.val[0]); i < rtable.rows; i++) {
                rtable.at<uchar>(i) = i;
            }
           /* cv::Mat table;
            cv::merge(std::array<cv::Mat, 3>{rtable}, table);*/
            cv::LUT(win, rtable, win);
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
    cv::Mat binary_show;
    cv::threshold(img, binary, 150, 1, cv::THRESH_BINARY);
    cv::threshold(img, binary_show, 150, 255, cv::THRESH_BINARY);
    // binaryPrint(binary);

    //cv::Moments m = cv::moments(binary, true);


    //std::cout << binary.type() << std::endl;

    cv::imshow("bin", binary_show);

    std::map<int, int> squares = binarySegmentation(binary);
    int max_key = 0;
    int max_value = -500;
    for (auto &el : squares) {
        if (el.second > max_value) {
            max_key = el.first;
            max_value = el.second;
        }
    }

    if (squares.empty()) {
        cv::circle(original, centroid, CIRCLE_RADIUS, BLUE_COLOR, cv::FILLED);
        cv::imshow("original", original);
        return {0, 0};
    }

    int m00 = 0;
    int m10 = 0;
    int m01 = 0;
    for (size_t i = 0; i < binary.rows; i++) {
        for (size_t j = 0; j < binary.cols; j++) {
            if (binary.at<uchar>(i, j) == max_key) {
                m00++;
                m10 += i;
                m01 += j;
            }
        }
    }

    if (m00 > 5) {
        centroid.x = m01 / m00;
        centroid.y = m10 / m00;
        color = RED_COLOR;
    } else {
        color = BLUE_COLOR;
    }

    cv::circle(original, centroid, CIRCLE_RADIUS, color, cv::FILLED);
    cv::imshow("original", original);

   return centroid;
}

int& at(std::map<int, int> &m, int key) {
    auto itr = m.find(key);
    if (itr == m.end()) {
        m[key] = 0;
    }
    return m[key];
}


std::map<int, int>  binarySegmentation(cv::Mat binary) {
    int km = 0;
    int kn = 0;
    int A = 0;
    int B = 0;
    int C = 0;
    int cur = 0;
    std::map<int, int> square;
    for(size_t i = 0; i < binary.rows; i++) {
        for (size_t j = 0; j < binary.cols; j++) {
            kn = j-1;
            if (kn <= 0) {
                kn = 0;
                B = 0;
            } else {
                B = binary.at<uchar>(i, kn);
            }
            km = i-1;
            if (km <= 0) {
                km = 0;
                C = 0;
            } else {
                C = binary.at<uchar>(km, j);
            }
            A = binary.at<uchar>(i, j);
            if (A == 0) {}
            else if (B == 0 && C == 0) {
                cur++;
                binary.at<uchar>(i, j) = cur;
                at(square, cur)++;
            } else if (B != 0 && C == 0) {
                binary.at<uchar>(i, j) = B;
                at(square, B)++;
            } else if (B == 0 && C != 0) {
                binary.at<uchar>(i, j) = C;
                at(square, C)++;
            } else if (B != 0 && C != 0) {
                at(square, B)++;
                if (B == C) {
                    binary.at<uchar>(i, j) = B;
                } else {
                    binary.at<uchar>(i, j) = B;
                    ///  Присвоить всем элементам С код B
//                    at(square, B) = at(square, B) + at(square, C);
//                    at(square, C) = 0;
                }
            }
        }
    }
    return std::move(square);
}


#ifdef ALGORITHM_TESTING
int main() {
    cv::VideoCapture cap("/media/nrx/ADATA UFD/ВКР/data/Пятно фрагмент 640х480.avi");
    if (!cap.isOpened()) {
        std::cout << "Ошибка открытия видео-источника" << std::endl;
        return 1;
    }
    cv::Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            break;
        }
        findRedPointCoordinates(frame);
        if (cv::waitKey(30) == ESC_KEY) {
            break;
        }
    }
    return 0;
}
#endif
