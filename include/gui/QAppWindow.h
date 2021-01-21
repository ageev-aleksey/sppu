//
// Created by nrx on 30.07.2020.
//

#ifndef TESTSIMULINKMODEL_MYAPP_H
#define TESTSIMULINKMODEL_MYAPP_H
#include <QtCore>
#include <QWidget>
#include "gui/QGridDynamicPlots.h"
#include <thread>
#include <atomic>
#include "gui/QSddView.h"
#include "gui/QSddModelSaver.h"

class QAppWindow : public QWidget
{
    Q_OBJECT
public:
    QAppWindow();
   ~QAppWindow() noexcept override;

public slots:
    void showModelDescribe();
    void saveModelParameters(SddModel::Parameters parameters);
private:
    void windowInit();
    void sddModelInit();

    //QModelPlots *plots;
    QSddView *mModel;
    QISddStateWidget *mSdd;
    QSddModelSaver *mSaver;
    QVBoxLayout *mLayout;
    QSettings mSettings;
    QWidget *modelDescribeWindow  = new QWidget;

};


#endif //TESTSIMULINKMODEL_MYAPP_H
