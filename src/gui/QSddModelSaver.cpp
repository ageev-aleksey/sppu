#include "gui/QSddModelSaver.h"
#include <QFileDialog>


QSddModelSaver::QSddModelSaver(const FormatsContainer<SddModelDescriptor> &formats, QWidget *parent)
: mFormats(formats), QWidget(parent) {
    mSaveButton->setText("Save model data");
    this->setLayout(mLayout);
    mLayout->addWidget(mSaveButton);
    QObject::connect(mSaveButton, &QPushButton::released, this, &QSddModelSaver::save);
}

void QSddModelSaver::addModelState(const SddModel::State &state) {
    if (isEnabled()) {
        mStates.outputs.push_back(state);
    }
}

void QSddModelSaver::setModelInput(const TimedInput &input) {
    if (isEnabled()) {
        // mInput = input;
    }
}

void QSddModelSaver::save() {
    QString path = QDir::currentPath() + "/data";
    QFileDialog dialog;
    dialog.setDefaultSuffix(*mFormats.names().begin());
    QString selectedFilter;
    QString filename = dialog.getSaveFileName(this, "save model states", path, filesTypeToString(), &selectedFilter);
    QString type = *selectedFilter.split(" ").begin();
    // QByteArray bytes =  mFormats.get(type)->write()
}

QString QSddModelSaver::filesTypeToString() {
    QString types;
    for (const auto &el : mFormats.names()) {
        types.append(el).append(" (").append("*.").append(el).append(");;");
    }
    return types;
}
