//#include <iostream>
//#include <opencv2/core/mat.hpp>
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/highgui.hpp>
//#include "opencv2/highgui/highgui.hpp"
//#include <fstream>
//int main() {
//    cv::Mat img_rgb, img_hsv, img_rgb_result;
//    //img_rgb = cv::imread("green_point.jpeg");
//    img_rgb = cv::imread("red3.jpg");
//    cv::cvtColor(img_rgb, img_hsv, cv::COLOR_BGR2HSV);
//    for(int i = 0; i < img_rgb.rows; i++) {
//        for(int j = 0; j < img_rgb.cols; j++) {
//            cv::Vec3b channels  = img_hsv.at<cv::Vec3b>(i, j);
//            /*if(!(channels[0] >=42 && channels[1] <= 127)) {
//                channels[1] = 0;
//                channels[2] = 0;
//            }*/
//
////            if(!(channels[0] < 15 && channels[1] > 100 && channels[2] >250)) {
////                channels[1] = 0;
////                channels[2] = 0;
////            }
//
//            if(!(channels[0] < 15 && channels[1] > 100  && channels[2] >225)) {
//                 int value = 0;
////                int value = channels[2];
////                value -= 150;
////                if (value < 0) {
////                    value = 0;
////                }
//                channels[2] = value;
//            }
//
//
//            img_hsv.at<cv::Vec3b>(i, j) = channels;
//        }
//    }
//    std::vector<uchar> buff;
//    cv::cvtColor(img_hsv, img_rgb_result, cv::COLOR_HSV2BGR);
//    cv::imencode(".jpg", img_rgb_result, buff);
//    std::ofstream  file("image.jpg");
//    for(auto &el : buff) {
//        file << el;
//    }
//    file.close();
//    cv::waitKey(0);
//    return 0;
//}

/////////////////////////////////////////////////

//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"
//
//#include <iostream>
//
//using namespace cv;
//using namespace std;
//
///// Global Variables
//Mat src_origin; Mat src; Mat hsv; Mat hue;
//int bins = 25;
//
///// Function Headers
//void Hist_and_Backproj(int, void* );
//
///** @function main */
//int main( int argc, char** argv )
//{
//    /// Read the image
//    src_origin = imread( argv[1]);
//    resize(src_origin, src, {1280, 720});
//    /// Transform it to HSV
//    cvtColor( src, hsv, COLOR_BGR2HSV );
//
//    /// Use only the Hue value
//    hue.create( hsv.size(), hsv.depth() );
//    int ch[] = { 0, 0 };
//    mixChannels( &hsv, 1, &hue, 1, ch, 1 );
//
//    /// Create Trackbar to enter the number of bins
//    char* window_image = "Source image";
//    namedWindow( window_image, WINDOW_AUTOSIZE );
//    createTrackbar("* Hue  bins: ", window_image, &bins, 180, Hist_and_Backproj );
//    Hist_and_Backproj(0, 0);
//
//    /// Show the image
//    imshow( window_image, src );
//
//    /// Wait until user exits the program
//    waitKey(0);
//    return 0;
//}
//
//
///**
// * @function Hist_and_Backproj
// * @brief Callback to Trackbar
// */
//void Hist_and_Backproj(int, void* )
//{
//    MatND hist;
//    int histSize = MAX( bins, 2 );
//    float hue_range[] = { 0, 180 };
//    const float* ranges = { hue_range };
//
//    /// Get the Histogram and normalize it
//    calcHist( &hue, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false );
//    normalize( hist, hist, 0, 255, NORM_MINMAX, -1, Mat() );
//
//    /// Get Backprojection
//    MatND backproj;
//    calcBackProject( &hue, 1, 0, hist, backproj, &ranges, 1, true );
//
//    /// Draw the backproj
//    imshow( "BackProj", backproj );
//
//    /// Draw the histogram
//    int w = 400; int h = 400;
//    int bin_w = cvRound( (double) w / histSize );
//    Mat histImg = Mat::zeros( w, h, CV_8UC3 );
//
//    for( int i = 0; i < bins; i ++ )
//    { rectangle( histImg, Point( i*bin_w, h ), Point( (i+1)*bin_w, h - cvRound( hist.at<float>(i)*h/255.0 ) ), Scalar( 0, 0, 255 ), -1 ); }
//
//    imshow( "Histogram", histImg );
//}

///////////////////////////////////////////////////
#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

const char ESCAPE_BUTTON_CODE = 27;

cv::Mat gamma_table() {
    cv::Mat table(1, 256, CV_8U);
    double alpha = 0.9;
    double gamma = 1.3;
    double invGamma = 1.0/gamma;

    for (double i = 0; i < 256.0; i++) {
        table.at<uchar>((int)i) = (uchar)(alpha*std::pow((i/255.0), invGamma) * 255.0);
    }
    return table;
}

cv::Mat make_table() {
    uchar alpha = 185;
    cv::Mat table(1, 256, CV_8U);
    for (int i = 0; i < alpha; i++) {
        table.at<uchar>(i) = 0;
    }
    int step = 256 - alpha;
    int value = 0;
    for (int i = alpha; i < 255; i++) {
        value += step;
        table.at<uchar>(i) = value;
    }
    return table;
}

int upperXRect = 0;
int upperYRect = 0;
int widthRect = 1600;
int heightRect = 1200;

cv::Mat imageOriginal;
cv::Mat imageBinary;
cv::Mat imageViewOriginal;
cv::Mat imageViewBinary;

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout <<"Usage: "<< argv[0] << " <путь до изображения>" << std::endl;
        return -1;
    }
    char *img_path = argv[1];
    cv::namedWindow("img", cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("upperX", "img", &upperXRect, 1600);
    cv::createTrackbar("upp erY", "img", &upperYRect, 1200);
    cv::createTrackbar("width", "img", &widthRect, 1600);
    cv::createTrackbar("height", "img", &heightRect, 1200);

    cv::VideoCapture cap(img_path, cv::CAP_FFMPEG);
    if (!cap.isOpened()) {
        std::cout << "Error opening [" << img_path << "]" << std::endl;
        return -1;
    }

    cv::Mat img;
    cv::Mat original;
    while (true) {
        cap >> original;

        if (original.empty()) {
            std::cout << "Error reading image [" << img_path << "]" << std::endl;
            return -1;
        }
        // auto gamma_t = gamma_table();
        img = original(cv::Rect(upperXRect, upperYRect, widthRect, heightRect));
       // cv::LUT(img, gamma_t, img);
        cv::GaussianBlur(img, img, {5, 5}, 0.5 ,0.5);

        std::vector<cv::Mat> channels;
        cv::split(img, channels);
        assert(channels.size() == 3);
        std::cout << "Channels size: " << channels.size() << std::endl;
        auto &blue = channels[0];
        auto &green = channels[1];
        auto &red = channels[2];

        auto table = make_table();
        cv::Mat ired;
        cv::LUT(red, table, ired);

        cv::Mat iimg;
        std::vector<cv::Mat> ichannels = {blue, green, ired};
        cv::merge(ichannels, iimg);

        cv::Mat hsv;
        cv::cvtColor(iimg, hsv, cv::COLOR_BGR2HSV);

        for (int i = 0; i < hsv.rows; i++) {
            for (int j = 0; j < hsv.cols; j++) {
                auto el = hsv.at<cv::Vec3b>(i, j);
                auto &h = el[0];
                auto &s = el[1];
                auto &v = el[2];
                if (!((h < 10) && s > 50 && v > 50)) {
                    v = 0;
                }
                hsv.at<cv::Vec3b>(i, j) = el;
            }
        }

        cv::Mat res;
        cv::Mat rgb;
        cv::Mat binary;
        cv::cvtColor(hsv, rgb, cv::COLOR_HSV2BGR);
        cv::cvtColor(rgb, res, cv::COLOR_BGR2GRAY);
        //cv::threshold(res, binary, 100, 255, cv::THRESH_BINARY);
        binary = res;
        auto moments = cv::moments(binary, false);
        cv::Point p(moments.m10/moments.m00, moments.m01/moments.m00);
        std::cout << "Centroid" << cv::Mat(p) << std::endl;
        cv::Mat resized;
        cv::resize(binary, resized, {1280, 720});

        imageOriginal = img;
        cv::circle(imageOriginal, p, 10, {0, 0, 255}, cv::FILLED);
        cv::resize(imageOriginal, imageViewOriginal, {640, 360});
        cv::resize(binary, imageViewBinary, {640, 360});
        cv::imshow("img", imageViewOriginal);
        cv::imshow("binary", imageViewBinary);
        if (cv::waitKey(20) == ESCAPE_BUTTON_CODE) {
            break;
        }
    }
    return 0;
}