//
// Created by nrx on 24.01.2021.
//

#ifndef SDDCLIENT_SHOT_H
#define SDDCLIENT_SHOT_H
#include <QtCore>
#include <QNetworkAccessManager>
#include <QNetworkReply>


class Shot : public QObject {
    Q_OBJECT
public:
    Shot(QObject *parent);
private slots:
    void data(QNetworkReply *reply);
private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
};

#endif //SDDCLIENT_SHOT_H
