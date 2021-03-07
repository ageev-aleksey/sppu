#include "gui/model_saver/QSddModelSaver.h"
#include "gui/model_saver/QModelDataGetter.h"
#include <QFileDialog>
#include <QtGlobal>


QSddModelSaver::QSddModelSaver(const FormatsContainer<SavingData> &formats,
                               std::shared_ptr<QModelDataGetter> getter,
                               QWidget *parent)
    : QWidget(parent), mFormats(formats), mSddConnection(std::move(getter))
{
    mSaveButton->setText("Save model data");
    this->setLayout(mLayout);
    mLayout->addWidget(mSaveButton);
    QObject::connect(mSaveButton, &QPushButton::released, this, &QSddModelSaver::save);
    mGetterConnection = QObject::connect(mSddConnection.get(), &QModelDataGetter::newData,
                                         this, &QSddModelSaver::addModelState);
}

void QSddModelSaver::addModelState(const SavingData &data) {
    if (mFile.isOpen()) {
        mStates.push_back(data);

        if (mStates.size() >= 5000) {
            QSddModelSaver::fileWrite();
        }
    }
}


void QSddModelSaver::save() {
    QString path = QDir::currentPath() + "/data";
    QFileDialog dialog;
    dialog.setDefaultSuffix(*mFormats.names().begin());
    QString selectedFilter;
    QString filename = dialog.getSaveFileName(this, "save model states", path, filesTypeToString(), &selectedFilter);
    QString type = *selectedFilter.split(" ").begin();
    filename += "." + type;
    if (mFile.isOpen()) {
        mFile.close();
    }
    mFile.setFileName(filename);
    mFile.open(QIODevice::OpenModeFlag::ReadWrite);
    if (!mFile.isOpen()) {
        qCritical() << "Ошибка открытия файла [" << filename << "] на запись";
    }
    auto fname = mFormats.names().first();
    auto format = mFormats.get(fname);
    mFile.write(format->begin());
    // QByteArray bytes =  mFormats.get(type)->write()
}

QString QSddModelSaver::filesTypeToString() {
    QString types;
    for (const auto &el : mFormats.names()) {
        types.append(el).append(" (").append("*.").append(el).append(");;");
    }
    return types;
}

QSddModelSaver::~QSddModelSaver() {
    if (mFile.isOpen()) {
        fileWrite();
        auto fname = mFormats.names().first();
        auto format = mFormats.get(fname);
        mFile.write(format->end());
        mFile.close();
    }
}

void QSddModelSaver::fileWrite() {
    auto fname =  mFormats.names().first();
    auto formatter = mFormats.get(fname);
    QByteArray result;
    for (const auto &el : mStates) {
        result.append(formatter->write(el));
        result.append(formatter->next());
    }
    mStates.clear();
    mFile.write(result);
}

void QSddModelSaver::setGetter(std::shared_ptr<QModelDataGetter> getter) {
    QObject::disconnect(mGetterConnection);
    mSddConnection = getter;
    mGetterConnection = QObject::connect(mSddConnection.get(), &QModelDataGetter::newData,
                                         this, &QSddModelSaver::addModelState);
}
