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

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

/// Global Variables
Mat src_origin; Mat src; Mat hsv; Mat hue;
int bins = 25;

/// Function Headers
void Hist_and_Backproj(int, void* );

/** @function main */
int main( int argc, char** argv )
{
    /// Read the image
    src_origin = imread( argv[1]);
    resize(src_origin, src, {1280, 720});
    /// Transform it to HSV
    cvtColor( src, hsv, COLOR_BGR2HSV );

    /// Use only the Hue value
    hue.create( hsv.size(), hsv.depth() );
    int ch[] = { 0, 0 };
    mixChannels( &hsv, 1, &hue, 1, ch, 1 );

    /// Create Trackbar to enter the number of bins
    char* window_image = "Source image";
    namedWindow( window_image, WINDOW_AUTOSIZE );
    createTrackbar("* Hue  bins: ", window_image, &bins, 180, Hist_and_Backproj );
    Hist_and_Backproj(0, 0);

    /// Show the image
    imshow( window_image, src );

    /// Wait until user exits the program
    waitKey(0);
    return 0;
}


/**
 * @function Hist_and_Backproj
 * @brief Callback to Trackbar
 */
void Hist_and_Backproj(int, void* )
{
    MatND hist;
    int histSize = MAX( bins, 2 );
    float hue_range[] = { 0, 180 };
    const float* ranges = { hue_range };

    /// Get the Histogram and normalize it
    calcHist( &hue, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false );
    normalize( hist, hist, 0, 255, NORM_MINMAX, -1, Mat() );

    /// Get Backprojection
    MatND backproj;
    calcBackProject( &hue, 1, 0, hist, backproj, &ranges, 1, true );

    /// Draw the backproj
    imshow( "BackProj", backproj );

    /// Draw the histogram
    int w = 400; int h = 400;
    int bin_w = cvRound( (double) w / histSize );
    Mat histImg = Mat::zeros( w, h, CV_8UC3 );

    for( int i = 0; i < bins; i ++ )
    { rectangle( histImg, Point( i*bin_w, h ), Point( (i+1)*bin_w, h - cvRound( hist.at<float>(i)*h/255.0 ) ), Scalar( 0, 0, 255 ), -1 ); }

    imshow( "Histogram", histImg );
}