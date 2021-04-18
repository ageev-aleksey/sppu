//
// Created by nrx on 04.08.2020.
//

#ifndef TESTSIMULINKMODEL_QSDDMODELSAVER_H
#define TESTSIMULINKMODEL_QSDDMODELSAVER_H
#include <QtCore>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include "gui/model_saver/SavingData.h"
#include "model/parser/SddModelDescriptor.h"
#include "format/FormatsContainer.h"
#include "sdd_protocol/connect/States.h"
#include "sdd_protocol/connect/QIConnection.h"
#include <unordered_map>
#include <string>

class QModelDataGetter;

// TODO (ageev) необходимо разнести способ записи данных в файл и интерфес доступа ка функционалу

/**
 * Виджет для сохранения данных об устройстве
 * на диске в некотором формате
 */
class QSddModelSaver : public QWidget {
    Q_OBJECT
public:
    /**
     *
     * @param formats - Объект определяющий формат хранения данных
     * @param getter - источник данных об устройстве
     * @param parent - родительский виджет qt
     */
    QSddModelSaver(const FormatsContainer<SavingData> &formats,
                   std::shared_ptr<QModelDataGetter> getter,
                   QWidget *parent = nullptr);
    /// Установка источника данных
    void setGetter(std::shared_ptr<QModelDataGetter> getter);
    ~QSddModelSaver() override;
public slots:
    /// Слот для добавления данных в буффер для записи на диск
    void addModelState(const SavingData &state);
private slots:
    /// Обработчки окна выбора пути сохранения данных
    void save();
private:
    /// запись данных в файл
    void fileWrite();
    /// формирования филтра файлов по его расширению
    QString filesTypeToString();
    /// буффер состояний
    std::vector<SavingData> mStates;
    /// Контейнир форматов доступный для сохранения
    FormatsContainer<SavingData> mFormats;
    QHBoxLayout *mLayout = new QHBoxLayout;
    QPushButton *mSaveButton = new QPushButton;
    /// Файл в который выполняется дозапись буффера
    QFile mFile;
    /// Источник данных
    std::shared_ptr<QModelDataGetter> mSddConnection;
    /// Соединение в EventLoop с источником данных
    QMetaObject::Connection mGetterConnection;
    // QMetaObject::Connection mChannelBufferWrite;

};
#endif //TESTSIMULINKMODEL_QSDDMODELSAVER_H
