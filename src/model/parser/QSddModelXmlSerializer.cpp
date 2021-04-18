#include "model/parser/QSddModelXmlSerializer.h"
#include <QJsonDocument>
#include <sstream>


QJsonObject QSddModelXmlSerializer::stateSerialize(const SddModel::State &state) {
    QJsonObject json;
    json["positionOx"] = state.positionOx;
    json["positionOz"] = state.positionOz;
    json["speedOx"] = state.speedOx;
    json["speedOz"] = state.speedOz;
    json["IOx"] = state.IOx;
    json["IOz"] = state.IOz;
    json["time"] = state.time;
    return json;
}

static void throw_if_dont_contain_required_keys(const QStringList &cKeys, const QStringList &rKeys) {
    for(const auto &el : rKeys) {
        if(!cKeys.contains(el)) {
            std::stringstream msg;
            msg << "Json error parsing SddModel::StatePackage; Json dont contain required field -> "
                << el.toStdString();
            throw std::runtime_error(msg.str());
        }
    }
}

SddModel::State QSddModelXmlSerializer::stateDeserialize(const QString &str) {
    QByteArray bytes;
    bytes.append(str);
    QJsonDocument doc = QJsonDocument::fromJson(bytes);
    if(!doc.isObject()) {
        throw std::runtime_error("Json error parsing SddModel::StatePackage; Json object expected");
    }
    auto jsonObject = doc.object();


    throw_if_dont_contain_required_keys(jsonObject.keys(), {"positionOx", "positionOz", "speedOx", "speedOz",
                                                         "IOx", "IOz", "time" });

    SddModel::State state{};

    state.positionOx = jsonObject["positionOx"].toDouble();
    state.positionOz = jsonObject["positionOz"].toDouble();
    state.speedOx = jsonObject["speedOx"].toDouble();
    state.speedOz = jsonObject["speedOz"].toDouble();
    state.IOx = jsonObject["IOx"].toDouble();
    state.IOz = jsonObject["IOz"].toDouble();
    state.time = jsonObject["time"].toDouble();
    return state;
}

QJsonObject QSddModelXmlSerializer::inputSerialize(const SddModel::Input &input) {
    QJsonObject json;
    json["pwmOx"] = input.pwmOx;
    json["pwmOz"] = input.pwmOz;

    return json;
}

SddModel::Input QSddModelXmlSerializer::inputDeserialize(const QString &str) {
    QByteArray bytes;
    bytes.append(str);
    QJsonDocument doc = QJsonDocument::fromJson(bytes);
    if(!doc.isObject()) {
        throw std::runtime_error("Json error parsing SddModel::StatePackage; Json object expected");
    }
    auto jsonObject = doc.object();

    throw_if_dont_contain_required_keys(jsonObject.keys(), {"pwmOx", "pwmOz",});

    SddModel::Input input{};
    input.pwmOx = jsonObject["pwmOx"].toDouble();
    input.pwmOz = jsonObject["pwmOz"].toDouble();
    return input;
}

#include <model/parser/QSddModelJsonSerializer.h>

QByteArray QSddModelXmlSerializer::write(SddModelDescriptor &&object) {
    auto parameters = parametersDeserialize(object.parameters);
    QJsonArray inputsArray;
    for(const auto &el : object.inputs) {
        QJsonObject tInput = inputSerialize(el.input);
        tInput["time"] = el.time;
        inputsArray.push_back(tInput);
    }
    QJsonArray outputsArray;
    for(const auto &el : object.outputs) {
        outputsArray.push_back(stateSerialize(el));
    }
    QJsonObject res;
    res["parameters"] = parameters;
    res["inputs"] = inputsArray;
    res["outputs"] = outputsArray;
    QJsonDocument doc;
    doc.setObject(res);
    return doc.toJson();
}

SddModelDescriptor QSddModelXmlSerializer::read(const QByteArray &data) {
    return SddModelDescriptor();
}

QJsonObject QSddModelXmlSerializer::parametersDeserialize(const SddModel::Parameters &parameters) {
    QJsonObject res;
    res["ox0"] = parameters.positionOx0;
    res["oz0"] = parameters.positionOz0;
    res["vx0"] = parameters.speedOx0;
    res["vz0"] = parameters.speedOz0;
    res["structCoeff"] = parameters.structCoeff;
    res["frictionCoeff"] = parameters.frictionCoeff;
    res["frictionLinearCoeff"] = parameters.frictionLinearCoeff;
    res["frictionQuadraticCoeff"] = parameters.frictionQuadraticCoeff;
    return res;
}

QSddModelXmlSerializer::QSddModelXmlSerializer() : Format<SddModelDescriptor>("xml") {}

