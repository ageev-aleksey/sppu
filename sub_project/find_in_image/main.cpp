#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <fstream>
int main() {
    cv::Mat img_rgb, img_hsv, img_rgb_result;
    //img_rgb = cv::imread("green_point.jpeg");
    img_rgb = cv::imread("red3.jpg");
    cv::cvtColor(img_rgb, img_hsv, cv::COLOR_BGR2HSV);
    for(int i = 0; i < img_rgb.rows; i++) {
        for(int j = 0; j < img_rgb.cols; j++) {
            cv::Vec3b channels  = img_hsv.at<cv::Vec3b>(i, j);
            /*if(!(channels[0] >=42 && channels[1] <= 127)) {
                channels[1] = 0;
                channels[2] = 0;
            }*/

//            if(!(channels[0] < 15 && channels[1] > 100 && channels[2] >250)) {
//                channels[1] = 0;
//                channels[2] = 0;
//            }

            if(!(channels[0] < 15 && channels[1] > 100  && channels[2] >225)) {
                 int value = 0;
//                int value = channels[2];
//                value -= 150;
//                if (value < 0) {
//                    value = 0;
//                }
                channels[2] = value;
            }


            img_hsv.at<cv::Vec3b>(i, j) = channels;
        }
    }
    std::vector<uchar> buff;
    cv::cvtColor(img_hsv, img_rgb_result, cv::COLOR_HSV2BGR);
    cv::imencode(".jpg", img_rgb_result, buff);
    std::ofstream  file("image.jpg");
    for(auto &el : buff) {
        file << el;
    }
    file.close();
    cv::waitKey(0);
    return 0;
}
