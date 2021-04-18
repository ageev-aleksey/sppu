//
// Created by nrx on 06.08.2020.
//
#include <QtCore>

#ifndef TESTSIMULINKMODEL_FORMAT_H
#define TESTSIMULINKMODEL_FORMAT_H


/**
 * Интерфейс сериализатора
 * @tparam T - тип объекта для которого выполняется сериализация
 */
template<typename T>
class Format {
public:
    /// Вернуть начальный набор байт для открытие хранилища объектов
    virtual QByteArray begin() = 0;
    /// Вернуть разделитьель между объектами
    virtual QByteArray next() = 0;
    /// Вернуть нр байт закрывающий хранилище объектов
    virtual QByteArray end() = 0;
    /// Преобразовать объект в набо байт
    virtual QByteArray write(const T &object) = 0;
    /// ПРочитать объект из байтов
    virtual T read(const QByteArray &data) = 0;
    /// Получить название формата
    QString name() {
        return mName;
    }
protected:
    /**
     *
     * @param name название формата
     */
    explicit Format(const QString &name) : mName(name) {}
    const QString mName;
};
#endif //TESTSIMULINKMODEL_FORMAT_H
