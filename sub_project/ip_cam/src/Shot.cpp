#include "Shot.h"
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

Shot::Shot(QObject *parent)
: QObject(parent), manager(new QNetworkAccessManager(this)),
request(QUrl("http://192.168.1.176:8080/shot.jpg"))
{
    QObject::connect(manager, &QNetworkAccessManager::finished, this, &Shot::data);
    manager->get(request);
}

void Shot::data(QNetworkReply *reply)
    {
        QByteArray buffer = reply->readAll();
        cv::Mat rawData(1, buffer.size(), CV_8UC1, (void*)buffer.data());
        cv::Mat decodedImage  =  cv::imdecode(rawData, 0);
        cv::imshow("web cam", decodedImage);
        manager->get(request);
    }
