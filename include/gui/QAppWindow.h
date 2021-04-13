//
// Created by nrx on 30.07.2020.
//

#ifndef TESTSIMULINKMODEL_MYAPP_H
#define TESTSIMULINKMODEL_MYAPP_H

#include "gui/QGridDynamicPlots.h"
#include "gui/camera/QCameraWindow.h"
#include "gui/QSddView.h"
#include "gui/model_saver/QSddModelSaver.h"
#include "gui/model_saver/QPointPositionInserter.h"

#include <QtCore>
#include <QWidget>

#include <thread>
#include <atomic>

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
    /// Выполнение соединение объектов через цикл событий
    void connectionInit();

    //QModelPlots *plots;
    QSddView *mModel;
    QISddStateWidget *mSdd;
    QSddModelSaver *mSaver;
    QVBoxLayout *mLayout;
    QSettings mSettings;
    QWidget *modelDescribeWindow  = new QWidget(this);
    QCameraWindow *camera = new QCameraWindow;
    std::shared_ptr<QPointPositionInserter> m_fullDataGetter;

};


#endif //TESTSIMULINKMODEL_MYAPP_H
