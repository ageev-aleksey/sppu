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


int main(int argc, char **argv) {
//    QCoreApplication a(argc, argv);
//    Shot *s = new Shot(&a);
//    return a.exec();
    cv::VideoCapture cap;
    cap.open("rtsp://192.168.1.176:5554/camera");
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
        if( cv::waitKey(1) >= 0 ){
            std::cout << "Escape pressed \n";
            break;
        }
    }
    return 0;
}