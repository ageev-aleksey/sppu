
#include "sdd_protocol/connect/QSddJsonSerializer.h"
#include <QJsonObject>
#include <stdexcept>

using namespace  sdd::conn;
using namespace std::chrono;


sdd::conn::QSddJsonSerializer::QSddJsonSerializer()
    : Format<State>("json")
{
    // empty
}


QByteArray QSddJsonSerializer::write(const State &object) {
    QJsonObject json;
    json["ox"] = object.ox;
    json["oy"] = object.oy;
    json["pwmX"] = object.pwm.ox;
    json["pwmY"] = object.pwm.oy;
    json["taskX"] = object.task.ox;
    json["taskY"] = object.task.oy;
    json["time"] = static_cast<long long int>(duration_cast<milliseconds>(object.time.time_since_epoch()).count());
    QJsonDocument doc;
    doc.setObject(json);
    return doc.toJson();
}

State QSddJsonSerializer::read(const QByteArray &data) {
    throw std::runtime_error("State QSddJsonSerializer::read(const QByteArray &data) : No implemented error!");
}

QByteArray QSddJsonSerializer::begin() {
    return "[";
}

QByteArray QSddJsonSerializer::end() {
    return "]";
}

QByteArray QSddJsonSerializer::next() {
    return ",";
}
