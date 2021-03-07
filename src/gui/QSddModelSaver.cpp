#include "gui/QSddModelSaver.h"
#include <QFileDialog>
#include <QtGlobal>


QSddModelSaver::QSddModelSaver(const FormatsContainer<sdd::conn::State> &formats,
                               std::shared_ptr<sdd::conn::QIConnection> connection,
                               QWidget *parent)
    : QWidget(parent), mFormats(formats), mSddConnection(std::move(connection))
{
    mSaveButton->setText("Save model data");
    this->setLayout(mLayout);
    mLayout->addWidget(mSaveButton);
    QObject::connect(mSaveButton, &QPushButton::released, this, &QSddModelSaver::save);
    QObject::connect(mSddConnection.get(), &sdd::conn::QIConnection::recvStatePackage,
                     this, &QSddModelSaver::addModelState);
}

void QSddModelSaver::addModelState(const sdd::conn::State &state) {
    if (mFile.isOpen()) {
        mStates.push_back(state);

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
