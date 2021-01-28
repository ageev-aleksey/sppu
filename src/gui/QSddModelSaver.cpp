#include "gui/QSddModelSaver.h"
#include <QFileDialog>
#include <QtGlobal>


QSddModelSaver::QSddModelSaver(const FormatsContainer<SddModelDescriptor> &formats,
                               std::shared_ptr<sdd::conn::IConnection> connection,
                               QWidget *parent)
    : QWidget(parent), mFormats(formats), mSddConnection(std::move(connection))
{
    mSaveButton->setText("Save model data");
    this->setLayout(mLayout);
    mLayout->addWidget(mSaveButton);
    QObject::connect(mSaveButton, &QPushButton::released, this, &QSddModelSaver::save);
    mSddConnection->addCallbackDataReady(
            [this](const sdd::conn::State &state) {
                if (mFile.isOpen()) {
                    mStates.push_back(state);
                    // TODO (ageev) организовать контоль размер буффера и реализовать
                    //  функцию WriteBlock, которая запишет весь буффер файл у класса FormatsContainer
                }
    })
}

void QSddModelSaver::addModelState(const sdd::conn::State &state) {
    if (isEnabled()) {
        mStates.push_back(state);
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


    // QByteArray bytes =  mFormats.get(type)->write()
}

QString QSddModelSaver::filesTypeToString() {
    QString types;
    for (const auto &el : mFormats.names()) {
        types.append(el).append(" (").append("*.").append(el).append(");;");
    }
    return types;
}
