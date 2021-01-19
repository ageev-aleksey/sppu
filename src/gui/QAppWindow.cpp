#include "gui/QAppWindow.h"
#include "gui/QSddModelControl.h"
#include "gui/QConditionalControl.h"
#include "gui/QISddStateWidgetFactory.h"
#include "model/parser/QSddModelJsonSerializer.h"
#include "model/parser/QSddModelXmlSerializer.h"
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
    QSddSerialPortControlFactory factory;
   // QSddModelControlFactory factory;
    auto sdd = factory.makeWidget(mSettings);
    mModel = new QSddView(sdd);
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



