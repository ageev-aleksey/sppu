#include "gui/QSddView.h"
#include <QGroupBox>
#include <QPushButton>


// TODO(ageev) SddController
QSddView::QSddView(QISddStateWidget *sddConnector, QWidget *parent)
: QWidget(parent), mSddConnect(sddConnector) {
    setLayout(mLayout);
    guiInit();
    QObject::connect(&mTimerGraphUpdate, &QTimer::timeout, this, &QSddView::stateInGui);
    mTimerGraphUpdate.start(40);
}


QSddView::~QSddView() noexcept {
    // TODO(aleksey) 1
}




void QSddView::guiInit() {
    //Графики состояния модели
    QGridDynamicPlots::Property p;
    p.limits[0].keyType = QGridDynamicPlots::Property::Limits::Type::WINDOW;
    p.limits[0].valueType = QGridDynamicPlots::Property::Limits::Type::AUTO;
    p.limits[0].value = {-1, 1};  // {-1, 1};
    p.limits[0].key = {10, 0};  // {0, 0.5};
    auto *plot = mPlots->addPlot(0, 0, &p);
    plot->plotLayout()->insertRow(0);
    plot->plotLayout()->addElement(0, 0,
                                   new QCPTextElement(plot, "Ox position", QFont("sans", 12, QFont::Weight::Bold)));
    plot = mPlots->addPlot(0, 1, &p);
    plot->plotLayout()->insertRow(0);
    plot->plotLayout()->addElement(0, 0,
                                   new QCPTextElement(plot, "Ox speed", QFont("sans", 12, QFont::Weight::Bold)));
    plot = mPlots->addPlot(1, 0, &p);
    plot->plotLayout()->insertRow(0);
    plot->plotLayout()->addElement(0, 0,
                                   new QCPTextElement(plot, "Oy position", QFont("sans", 12, QFont::Weight::Bold)));
    plot = mPlots->addPlot(1, 1, &p);
    plot->plotLayout()->insertRow(0);
    plot->plotLayout()->addElement(0, 0,
                                   new QCPTextElement(plot, "Oy speed", QFont("sans", 12, QFont::Weight::Bold)));

    plot = mPlots->addPlot(0, 2, &p);
    plot->plotLayout()->insertRow(0);
    plot->plotLayout()->addElement(0, 0,
                                   new QCPTextElement(plot, "Ox input", QFont("sans", 12, QFont::Weight::Bold)));

    plot = mPlots->addPlot(1, 2, &p);
    plot->plotLayout()->insertRow(0);
    plot->plotLayout()->addElement(0, 0,
                                   new QCPTextElement(plot, "Oy input", QFont("sans", 12, QFont::Weight::Bold)));
    mLayout->addWidget(mPlots, 1);

    mLayout->addWidget(mSddConnect);
}


void QSddView::stateInGui() {
    std::vector<sdd::conn::State> state = mSddConnect->getSddStates();
    if (!state.empty()) {
        auto &endState = *(--state.end());
        auto &centerState = state[state.size()/2];
        auto &beginState = state[0];

        double beginTime = std::chrono::duration_cast<std::chrono::milliseconds>(beginState.time.time_since_epoch())
                .count() / 1000.0;
        double centerTime = std::chrono::duration_cast<std::chrono::milliseconds>(centerState.time.time_since_epoch())
                .count() / 1000.0;
        double endTime = std::chrono::duration_cast<std::chrono::milliseconds>(endState.time.time_since_epoch())
                .count() / 1000.0;

        mPlots->addPlotData(0, 0, 0, beginTime, beginState.ox);
        mPlots->addPlotData(0, 0, 0, centerTime, centerState.ox);
        mPlots->addPlotData(0, 0, 0, endTime, endState.ox);


        double beforeTime = std::chrono::duration_cast<std::chrono::milliseconds>(mBeforeState.time.time_since_epoch())
                                    .count() / 1000.0;
        double speedOx = (endState.ox - mBeforeState.ox)/(endTime - beforeTime);
        double speedOy = (endState.oy - mBeforeState.oy)/(endTime - beforeTime);
       // mPlots->addPlotData(0, 1, 0, beginTime, beginState.speedOx);
        // mPlots->addPlotData(0, 1, 0, centerTime, centerState.speedOx);
        mPlots->addPlotData(0, 1, 0, endTime, speedOx);

        mPlots->addPlotData(1, 0, 0, beginTime, beginState.oy);
        mPlots->addPlotData(1, 0, 0, centerTime, centerState.oy);
        mPlots->addPlotData(1, 0, 0, endTime, endState.oy);

        mPlots->addPlotData(1, 1, 0, beginTime, speedOy);
//        mPlots->addPlotData(1, 1, 0, centerTime, centerState.speedOz);
//        mPlots->addPlotData(1, 1, 0, endTime, endState.speedOz);

        mPlots->addPlotData(0, 2, 0, beginTime, beginState.pwm.ox);
        mPlots->addPlotData(0, 2, 0, centerTime, centerState.pwm.ox);
        mPlots->addPlotData(0, 2, 0, endTime, endState.pwm.ox);

        mPlots->addPlotData(1, 2, 0, beginTime, beginState.pwm.oy);
        mPlots->addPlotData(1, 2, 0, centerTime, centerState.pwm.oy);
        mPlots->addPlotData(1, 2, 0, endTime, endState.pwm.oy);



        mPlots->allReplot();
        mBeforeState = endState;
//        mXPosition->setText(QString::number(endState.ox));
//        mZPosition->setText(QString::number(endState.oy));
//        mXSpeed->setText(QString::number(endState.speedOx));
//        mZSpeed->setText(QString::number(endState.speedOz));
    }

//    if (!state.empty()) {
//        for (const auto &el : state) {
//            mPlots->addPlotData(0, 0, 0, el.time, el.positionOx);
//            mPlots->addPlotData(0, 1, 0, el.time, el.speedOx);
//            mPlots->addPlotData(1, 0, 0, el.time, el.positionOz);
//            mPlots->addPlotData(0, 2, 0, el.time, el.IOx);
//            mPlots->addPlotData(1, 2, 0, el.time, el.IOz);
//            mPlots->allReplot();
//        }
//        mXPosition->setText(QString::number(state.back().positionOx));
//        mZPosition->setText(QString::number(state.back().positionOz));
//        mXSpeed->setText(QString::number(state.back().speedOx));
//        mZSpeed->setText(QString::number(state.back().speedOz));
//    }
}



void QSddView::setModelInput(const SddModel::Input &input) {
  //  mSddConnect->setInput(input);
}

QISddStateWidget* QSddView::setSddStateWidget(QISddStateWidget *sddConnector) {
    mLayout->replaceWidget(mSddConnect, sddConnector);
    auto *tmp = mSddConnect;
    mSddConnect = sddConnector;
    return tmp;
}



