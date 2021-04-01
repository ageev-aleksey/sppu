//
// Created by nrx on 30.07.2020.
//

#ifndef TESTSIMULINKMODEL_MYAPP_H
#define TESTSIMULINKMODEL_MYAPP_H
#include <QtCore>
#include <QWidget>
#include "gui/QGridDynamicPlots.h"
#include "gui/camera/QCameraWindow.h"
#include <thread>
#include <atomic>
#include "gui/QSddView.h"
#include "gui/model_saver/QSddModelSaver.h"

/**
 * Главный класс приложения.
 * Создает и компанует все окна
 */
class QAppWindow : public QWidget
{
    Q_OBJECT
public:
    QAppWindow();
   ~QAppWindow() noexcept override;

public slots:
    /// Отображение описание модели, заложенное в программу
    void showModelDescribe();
    /// Сохранение всех параметров программы
    void saveModelParameters(SddModel::Parameters parameters);
    /// Отображение окна подключения камеры
    void showCameraOptions();
private:
    /// Инициализация главного окна
    void windowInit();
    /// Инициализация параметров подключения
    /// Подключение к устройству или замещение устройтсва моделью
    void sddModelInit();

    //QModelPlots *plots;
    QSddView *mModel;
    QISddStateWidget *mSdd;
    QSddModelSaver *mSaver;
    QVBoxLayout *mLayout;
    QSettings mSettings;
    QWidget *modelDescribeWindow  = new QWidget(this);
    QCameraWindow *camera = new QCameraWindow;

};


#endif //TESTSIMULINKMODEL_MYAPP_H
