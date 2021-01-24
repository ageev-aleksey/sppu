#include "Shot.h"
#include <iostream>
#include <QtCore>
#include <QNetworkAccessManager>
#include <QNetworkReply>


int main(int argc, char **argv) {
    QCoreApplication a(argc, argv);
    Shot *s = new Shot(&a);
    return a.exec();
//    cv::VideoCapture cam("http://user:user@192.168.1.176:8080/video?");
//    if (!cam.isOpened()) {
//        std::cout << "Error opening cam" << std::endl;
//        return -1;
//    }


//    for(;;) {
//        QNetworkReply *repl = nw.get(request);
//        QByteArray buffer = repl->readAll();
//        cv::Mat rawData(1, buffer.size(), CV_8UC1, (void*)buffer.data());
//        cv::Mat decodedImage  =  cv::imdecode(rawData, 0);
////        if (frame.empty()) {
////            std::cout << "Frame is empty. Exit" << std::endl;
////            break;
////        }
//        cv::imshow("web cam", decodedImage);
//        if (cv::waitKey(1) == 27) {
//            break;
//        }
//    }
}