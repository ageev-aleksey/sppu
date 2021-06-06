
#include "model/QModelOutput.h"
#include "model/PwdInputGenerator.h"

#include <QtCore/QCoreApplication>
#include <QtCore>
#include <memory>
#include <exception>
#include <sstream>
#include <QtSerialPort/QSerialPort>

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;
/**
 * Создание модели работаюющая в отдельном потоке, которая отправлет данные в устройство
 * @param file_path - путь до файла с параметрами модели и настройками устройства в json формате
 * @return
 */
std::unique_ptr<QModelOutput> make(const QString &file_path);
/**
 * Вывод параметров модели на экран
 * @param params - параметры моедли
 */
void paramsPrint(const SddModel::Parameters &params);
/**
 * Извлечение из JsonObject параметров для модели
 * @param obj
 * @return
 */
SddModel::Parameters modelParametersParse(const QJsonObject &obj);
QString deviceParse(const QJsonObject &obj);

int main(int argc, char **argv) {
    std::cout << fs::current_path() << std::endl;
    QCoreApplication appCore(argc, argv);
    qSetMessagePattern("[%{time yyyyMMdd h:mm:ss.zzz t} "
                      "%{if-debug}D%{endif}"
                      "%{if-info}I%{endif}"
                      "%{if-warning}W%{endif}"
                      "%{if-critical}C%{endif}"
                      "%{if-fatal}F%{endif}]"
                      " %{file}:%{line} - %{message}");
    qInfo() << "App startup...";
    auto modelOut = make("model_config.json");
    return QCoreApplication::exec();
}



std::unique_ptr<QModelOutput> make(const QString &file_path) {
    std::cout << file_path.toStdString() << std::endl;
    QFile property(file_path);
    if (!property.open(QIODevice::ReadOnly)) {
        QString msg;
        msg.append("Error opening file [").append(file_path).append("]");
        qCritical() << msg;
        throw std::runtime_error(msg.toStdString());
    } else {
        qInfo() << "File [" << file_path << "] opened success";
    }
    QByteArray jsonValue = property.readAll();
    QJsonParseError err{};
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonValue, &err);
    if (err.error != QJsonParseError::NoError) {
        QString msg;
        msg.append("Error parsing json. ").append(err.errorString());
        qCritical() << msg;
        throw std::runtime_error(msg.toStdString());
    }

    if (!jsonDoc.isObject()) {
        QString str("Invalid json format. Json is not a JsonObject");
        qCritical() <<str;
        throw std::runtime_error(str.toStdString());
    }
    auto obj = jsonDoc.object();
    auto sddParamsJson = obj["sddModel"];
    if (!sddParamsJson.isObject()) {
        throw std::runtime_error("field sddModel not object");
    }

    SddModel::Parameters params = modelParametersParse(sddParamsJson.toObject());

    auto comDeviceJson = obj["device"];
    if (!comDeviceJson.isObject()) {
        throw std::runtime_error("field comDevice not object");
    }
    QString devicePath = deviceParse(comDeviceJson.toObject());

    paramsPrint(params);
    qInfo() << "conDevicePath: " << devicePath;

    auto device = std::make_unique<QSerialPort>(devicePath);
    if (!device->open(QIODevice::WriteOnly)) {
        std::stringstream msg;
        msg << "Error opening device [" << devicePath.toStdString() << "] -> " << device->error();
        qCritical() << msg.str().c_str();
        throw std::runtime_error(msg.str());
    }


    auto modelExecutor = std::make_unique<QSddModelExecutor>(std::make_unique<SddModel>());
    modelExecutor->setParameters(params);
    modelExecutor->brakingModeling(50);
   // modelExecutor->brakingModeling(1000);
    auto pwdGenerator = std::make_shared<PwdInputGenerator>(1, -1, 1);
    pwdGenerator->setDutyCycle(0.5, 0.5);
    modelExecutor->setInputGenerator(pwdGenerator);
    auto modelOutput = std::make_unique<QModelOutput>(std::move(modelExecutor), std::move(device));
    return std::move(modelOutput);
}


void paramsPrint(const SddModel::Parameters &params) {
    qInfo() << "SddModel::Parameters {\n"
            << "speedOx0:" << params.speedOx0 << "\n"
            << "speedOz0:" << params.speedOz0 << "\n"
            << "positionOx0:" << params.positionOx0 << "\n"
            << "positionOz0:" << params.positionOz0 << "\n"
            << "frictionCoeff:" << params.frictionCoeff << "\n"
            << "frictionLinearCoeff:" << params.frictionLinearCoeff << "\n"
            << "frictionQuadraticCoeff:" << params.frictionQuadraticCoeff << "\n"
            << "structCoeff:" << params.structCoeff << "}";
}

std::string jsonTypeToString(QJsonValue::Type type) {
    switch (type) {

        case QJsonValue::Null:
            return "Null";
        case QJsonValue::Bool:
            return "Bool";
        case QJsonValue::Double:
            return "Double";
        case QJsonValue::String:
            return "String";
        case QJsonValue::Array:
            return "Array";
        case QJsonValue::Object:
            return "Object";
        case QJsonValue::Undefined:
        default:
            return "Undefined";
    }
}

void checkJsonFieldTypeDouble(const QJsonObject &obj, const QString &field) {
    if (obj[field].isNull()) {
        std::stringstream msg;
        msg << "Json object don't containing " << field.toStdString() << "field";
        throw std::runtime_error(msg.str());
    }
    if (!obj[field].isDouble()) {
        std::stringstream msg;
        msg << "Field [" << field.toStdString() << "] invalid type [" << jsonTypeToString(obj["speedOx0"].type()) << "]";
        throw std::runtime_error(msg.str());
    }
}

SddModel::Parameters modelParametersParse(const QJsonObject &obj) {
    checkJsonFieldTypeDouble(obj, "speedOx0");
    checkJsonFieldTypeDouble(obj, "speedOz0");
    checkJsonFieldTypeDouble(obj, "positionOx0");
    checkJsonFieldTypeDouble(obj, "positionOz0");
    checkJsonFieldTypeDouble(obj, "frictionLinearCoeff");
    checkJsonFieldTypeDouble(obj, "frictionQuadraticCoeff");
    checkJsonFieldTypeDouble(obj, "structCoeff");

    SddModel::Parameters ret{};
    ret.speedOz0 = obj["speedOz0"].toDouble();
    ret.positionOx0 = obj["positionOx0"].toDouble();
    ret.positionOz0 = obj["positionOz0"].toDouble();
    ret.frictionCoeff = obj["frictionCoeff"].toDouble();
    ret.frictionLinearCoeff = obj["frictionLinearCoeff"].toDouble();
    ret.frictionQuadraticCoeff = obj["frictionQuadraticCoeff"].toDouble();
    ret.structCoeff = obj["structCoeff"].toDouble();

    ret.speedOz0 = 0;
    ret.positionOx0 = 0;
    ret.positionOz0 = 0;
    ret.frictionCoeff = 0.000001;
    ret.frictionLinearCoeff = 1;
    ret.frictionQuadraticCoeff = 0.001;
    ret.structCoeff = 15000;
    return ret;
}

QString deviceParse(const QJsonObject &obj) {
    if (!obj["type"].isString()) {
        throw std::runtime_error("Invalid type field with name type");
    }
    QString typeValue = obj["type"].toString();
    if (typeValue != "comport") {
        std::stringstream msg;
        msg << "invalid field type value: " << typeValue.toStdString();
        throw std::runtime_error(msg.str());
    }

    if (!obj["path"].isString()) {
        throw std::runtime_error("Invalid type field with name path");
    }
    return obj["path"].toString();
}