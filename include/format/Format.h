//
// Created by nrx on 06.08.2020.
//
#include <QtCore>

#ifndef TESTSIMULINKMODEL_FORMAT_H
#define TESTSIMULINKMODEL_FORMAT_H
template<typename T>
class Format {
public:
    virtual QByteArray begin() = 0;
    virtual QByteArray next() = 0;
    virtual QByteArray end() = 0;
    virtual QByteArray write(const T &object) = 0;
    virtual T read(const QByteArray &data) = 0;
    QString name() {
        return mName;
    }
protected:
    explicit Format(const QString &name) : mName(name) {}
    const QString mName;
};
#endif //TESTSIMULINKMODEL_FORMAT_H
