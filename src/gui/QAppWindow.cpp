#include "gui/QAppWindow.h"
#include "gui/QSddModelControl.h"
#include "gui/QConditionalControl.h"
#include "gui/QISddStateWidgetFactory.h"
#include "model/parser/QSddModelJsonSerializer.h"
#include "model/parser/QSddModelXmlSerializer.h"
#include "sdd_protocol/connect/QSddJsonSerializer.h"
#include "gui/model_saver/QIConnectionAdapter.h"
#include "gui/camera/QCameraWindow.h"
#include <iostream>
#include <qcustomplot.h>



QAppWindow::QAppWindow() :  mSettings("AVV", "TestApp") {
        windowInit();
        sddModelInit();
}

QAppWindow::~QAppWindow() noexcept {
    QSize s = size();
    mSettings.setValue("window/height", s.height());
    mSettings.setValue("window/width", s.width());
    mSdd->settingsStore(mSettings);
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
    auto file_menu = menu->addMenu("file");
    file_menu->addAction("show full model describe", this, &QAppWindow::showModelDescribe);
    file_menu->addAction("connect to camera", this, &QAppWindow::showCameraOptions);
    // QObject::connect(describeModel, &QMenu::triggered, this, &QAppWindow::showModelDescribe);
    QLabel *img = new QLabel;
    img->setPixmap(QPixmap(":/model.png"));
    QVBoxLayout *l = new QVBoxLayout;
    l->addWidget(img);
    modelDescribeWindow->setLayout(l);
    mLayout->addWidget(menu);
}

void QAppWindow::sddModelInit() {
    QSddSerialPortControlFactory factory;
   // QSddModelControlFactory factory;
    mSdd = factory.makeWidget(mSettings);
    mModel = new QSddView(mSdd);
    FormatsContainer<SavingData> f;
    f.add(std::make_shared<sdd::conn::QSddJsonSerializer>());
//    f.add(std::make_shared<QSddModelXmlSerializer>());
    mSaver = new QSddModelSaver(f, std::make_shared<QIConnectionAdapter>(mSdd->getSddConnection()));
    mLayout->addWidget(mModel, 1);
    auto *tab = new QTabWidget;
    // auto *page = new QConditionalControl;
    // page->addWidget(mSaver);
    tab->addTab(mSaver, "save to file");
    mLayout->addWidget(tab, 0);
}

void QAppWindow::showModelDescribe() {
    modelDescribeWindow->show();
}

void QAppWindow::saveModelParameters(SddModel::Parameters parameters) {
//    mSettings.setValue("model/positionOx0", parameters.positionOx0);
//    mSettings.setValue("model/positionOz0", parameters.positionOz0);
//    mSettings.setValue("model/speedOx0", parameters.speedOx0);
//    mSettings.setValue("model/speedOz0", parameters.speedOz0);
//    mSettings.setValue("model/structCoeff", parameters.structCoeff);
//    mSettings.setValue("model/frictionCoeff", parameters.frictionCoeff);
//    mSettings.setValue("model/frictionLinearCoeff", parameters.frictionLinearCoeff);
//    mSettings.setValue("model/frictionQuadraticCoeff", parameters.frictionQuadraticCoeff);

}

void QAppWindow::showCameraOptions() {
    auto cameraOptions = new QCameraWindow();
    cameraOptions->show();
}



