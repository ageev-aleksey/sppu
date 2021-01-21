#include "gui/QISddStateWidgetFactory.h"
#include "gui/QSddSerialPortControl.h"
#include <QtSerialPort>

QISddStateWidget *QSddModelControlFactory::makeWidget(QSettings &settings) {
    auto model = std::make_unique<QSddModelExecutor>(std::make_unique<SddModel>());
    // QObject::connect(model.get(), &QSddModelExecutor::parametersUpdate, this, &QAppWindow::saveModelParameters);

    auto param = model->getParameters();
    bool okRead = true;
    bool readRes = true;
    param.positionOx0 = settings.value("model/positionOx0", param.positionOx0).toDouble(&okRead);
    readRes &= okRead;
    param.positionOz0 = settings.value("model/positionOz0", param.positionOz0).toDouble(&okRead);
    readRes &= okRead;
    param.speedOx0 = settings.value("model/speedOx0", param.speedOx0).toDouble(&okRead);
    readRes &= okRead;
    param.speedOz0 = settings.value("model/speedOz0", param.speedOz0).toDouble(&okRead);
    readRes &= okRead;
    param.structCoeff = settings.value("model/structCoeff", param.structCoeff).toDouble(&okRead);
    readRes &= okRead;
    param.frictionCoeff = settings.value("model/frictionCoeff", param.frictionCoeff).toDouble(&okRead);
    readRes &= okRead;
    param.frictionLinearCoeff =
            settings.value("model/frictionLinearCoeff", param.frictionLinearCoeff).toDouble(&okRead);
    readRes &= okRead;
    param.frictionQuadraticCoeff =
            settings.value("model/frictionQuadraticCoeff", param.frictionQuadraticCoeff).toDouble(&okRead);
    readRes &= okRead;
    if (readRes) {
        model->setParameters(param);
    } else {
        auto *errorParamRead =
                new QLabel("Error parameters of model reading from config. Using default model parameters");
        auto p(errorParamRead->palette());
        p.setColor(QPalette::ColorRole::Text, Qt::GlobalColor::red);
        errorParamRead->setPalette(p);
      //  mLayout->addWidget(errorParamRead);
    }
    return new QSddModelControl(std::move(model));
}

QISddStateWidget *QSddSerialPortControlFactory::makeWidget(QSettings &settings) {
    auto port = std::make_unique<QSerialPort>();
    port->setFlowControl(QSerialPort::NoFlowControl);
    port->setParity(QSerialPort::Parity::NoParity);
    port->setBaudRate(QSerialPort::BaudRate::Baud38400);
    port->setDataBits(QSerialPort::DataBits::Data7);
    auto *widget = new QSddSerialPortControl(std::move(port));;
    widget->settingsLoad(settings);
    return widget;
}
