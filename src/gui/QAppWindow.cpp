
#include "gui/QAppWindow.h"
#include <qcustomplot.h>
#include <iostream>
#include "gui/QConditionalControl.h"
#include "model/parser/QSddModelJsonSerializer.h"
#include "model/parser/QSddModelXmlSerializer.h"


QAppWindow::QAppWindow() :  mSettings("AVV", "TestApp") {
        windowInit();
        sddModelInit();
}

QAppWindow::~QAppWindow() noexcept {
    QSize s = size();
    mSettings.setValue("window/height", s.height());
    mSettings.setValue("window/width", s.width());
    mSettings.sync();
}


void QAppWindow::windowInit() {
    setWindowTitle("Sdd model");
    int width = mSettings.value("window/width", 300).toInt();
    int height = mSettings.value("window/height", 500).toInt();
    resize(width, height);
    mLayout =  new QVBoxLayout;
    setLayout(mLayout);
    auto *menu = new QMenuBar();
    auto describeModel = menu->addMenu("file");
    describeModel->addAction("show full model describe", this, &QAppWindow::showModelDescribe);
    // QObject::connect(describeModel, &QMenu::triggered, this, &QAppWindow::showModelDescribe);
    QLabel *img = new QLabel;
    img->setPixmap(QPixmap(":/model.png"));
    QVBoxLayout *l = new QVBoxLayout;
    l->addWidget(img);
    modelDescribeWindow->setLayout(l);
    mLayout->addWidget(menu);
}

void QAppWindow::sddModelInit() {
    auto model = std::make_unique<QSddModelExecutor>(std::make_unique<SddModel>());
    QObject::connect(&(*model), &QSddModelExecutor::parametersUpdate, this, &QAppWindow::saveModelParameters);

    auto param = model->getParameters();
    bool okRead = true;
    bool readRes = true;
    param.positionOx0 = mSettings.value("model/positionOx0", param.positionOx0).toDouble(&okRead);
    readRes &= okRead;
    param.positionOz0 = mSettings.value("model/positionOz0", param.positionOz0).toDouble(&okRead);
    readRes &= okRead;
    param.speedOx0 = mSettings.value("model/speedOx0", param.speedOx0).toDouble(&okRead);
    readRes &= okRead;
    param.speedOz0 = mSettings.value("model/speedOz0", param.speedOz0).toDouble(&okRead);
    readRes &= okRead;
    param.structCoeff = mSettings.value("model/structCoeff", param.structCoeff).toDouble(&okRead);
    readRes &= okRead;
    param.frictionCoeff = mSettings.value("model/frictionCoeff", param.frictionCoeff).toDouble(&okRead);
    readRes &= okRead;
    param.frictionLinearCoeff =
            mSettings.value("model/frictionLinearCoeff", param.frictionLinearCoeff).toDouble(&okRead);
    readRes &= okRead;
    param.frictionQuadraticCoeff =
            mSettings.value("model/frictionQuadraticCoeff", param.frictionQuadraticCoeff).toDouble(&okRead);
    readRes &= okRead;
    if (readRes) {
        model->setParameters(param);
    } else {
        auto *errorParamRead =
                new QLabel("Error parameters of model reading from config. Using default model parameters");
        auto p(errorParamRead->palette());
        p.setColor(QPalette::ColorRole::Text, Qt::GlobalColor::red);
        errorParamRead->setPalette(p);
        mLayout->addWidget(errorParamRead);
    }

    mModel = new QSddModelView(std::move(model));
    FormatsContainer<SddModelDescriptor> f;
    f.add(std::make_shared<QSddModelJsonSerializer>());
    f.add(std::make_shared<QSddModelXmlSerializer>());
    mSaver = new QSddModelSaver(f);
    mLayout->addWidget(mModel, 1);
    auto *tab = new QTabWidget;
    auto *page = new QConditionalControl;
    page->addWidget(mSaver);
    tab->addTab(page, "save to file");
    mLayout->addWidget(tab, 0);
}

void QAppWindow::showModelDescribe() {
    modelDescribeWindow->show();
}

void QAppWindow::saveModelParameters(SddModel::Parameters parameters) {
    mSettings.setValue("model/positionOx0", parameters.positionOx0);
    mSettings.setValue("model/positionOz0", parameters.positionOz0);
    mSettings.setValue("model/speedOx0", parameters.speedOx0);
    mSettings.setValue("model/speedOz0", parameters.speedOz0);
    mSettings.setValue("model/structCoeff", parameters.structCoeff);
    mSettings.setValue("model/frictionCoeff", parameters.frictionCoeff);
    mSettings.setValue("model/frictionLinearCoeff", parameters.frictionLinearCoeff);
    mSettings.setValue("model/frictionQuadraticCoeff", parameters.frictionQuadraticCoeff);
}



