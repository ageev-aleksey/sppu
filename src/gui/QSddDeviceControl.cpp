#include "gui/QSddDeviceControl.h"


QSddDeviceControl::QSddDeviceControl(std::unique_ptr<QSerialPort> connection)
{
    m_pSender =
    setLayout(mLayout);
}

std::vector<sdd::conn::State> QSddDeviceControl::getSddStates() {
    return std::vector<sdd::conn::State>();
}

void QSddDeviceControl::guiInit() {
    auto *dashBoardLayout = new QHBoxLayout;

    auto textDeviceStateLayout = new QGridLayout;
    auto *box = new QGroupBox("Current device state");
    box->setLayout(textDeviceStateLayout);
    textDeviceStateLayout->addWidget(new QLabel("X position:"), 0, 0);
    textDeviceStateLayout->addWidget(new QLabel("Z position:"), 1, 0);
    textDeviceStateLayout->addWidget(new QLabel("X speed:"), 2, 0);
    textDeviceStateLayout->addWidget(new QLabel("Z speed:"), 3, 0);
    textDeviceStateLayout->addWidget(mXPosition, 0, 1);
    textDeviceStateLayout->addWidget(mYPosition, 1, 1);
    textDeviceStateLayout->addWidget(mXSpeed, 2, 1);
    textDeviceStateLayout->addWidget(mYSpeed, 3, 1);

    dashBoardLayout->addLayout(textDeviceStateLayout);

    auto *controlLayout = new QGridLayout;

    auto formLayout = new QFormLayout;
    formLayout->addRow(new QLabel("Task Ox:"), mOxTask);
    formLayout->addRow(new QLabel("Task Oy"), mOyTask);
    controlLayout->addLayout(formLayout, 0, 0);

    formLayout = new QFormLayout;
    formLayout->addRow(new QLabel("Pwm Ox:"), mPwmOx);
    formLayout->addRow(new QLabel("Pwm Oy:"), mPwmOy);
    formLayout->addRow(new QLabel("Light:"), mLightBlinking);
    controlLayout->addLayout(formLayout, 0, 1);
    mControlWidget->setLayout(controlLayout);

    QTabWidget *tabs = new QTabWidget;
    tabs->addTab(mControlWidget, "Control");
    mLayout->addWidget(tabs);
}
