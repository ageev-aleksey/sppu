
#include "sdd_protocol/connect/QSddJsonSerializer.h"
#include <QJsonObject>
#include <stdexcept>

using namespace  sdd::conn;
using namespace std::chrono;


sdd::conn::QSddJsonSerializer::QSddJsonSerializer()
    : Format<SavingData>("json")
{
    // empty
}


QByteArray QSddJsonSerializer::write(const SavingData &object) {
    QJsonObject json;
    json["ox"] = object.state.ox;
    json["oy"] = object.state.oy;
    json["pwmX"] = object.state.pwm.ox;
    json["pwmY"] = object.state.pwm.oy;
    json["taskX"] = object.state.task.ox;
    json["taskY"] = object.state.task.oy;
    json["time"] = static_cast<long long int>(duration_cast<milliseconds>(object.state.time.time_since_epoch()).count());

    QJsonObject additional;
    for (const auto &el : object.additional) {
        additional[el.first.c_str()] = el.second.c_str();
    }

    QJsonDocument doc;
    doc.setObject(json);
    return doc.toJson();
}

SavingData QSddJsonSerializer::read(const QByteArray &data) {
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
