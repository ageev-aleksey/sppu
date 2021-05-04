#include "gui/camera/QCameraWindow.h"
#include "camera/QCvVcCamera.h"
#include "camera/QContourHDCamera.h"
#include <QLineEdit>
#include <QPushButton>
#include <QCloseEvent>

const QString IMG_SOURCE_CVCAP = "cv::VideoCapture";



class QCameraWindowDialogContentImpl : public QWidget {
    Q_OBJECT
public:
    QCameraWindowDialogContentImpl() {
//        cvLayout->addWidget(videoSourceURL);
//        cvLayout->addWidget(button);
        button->setText("Connect");
    }


    // - Для cv::VideoCapture
    // -- поле настроек
    QLineEdit *videoSourceURL = new QLineEdit(this);

    QPushButton *button = new QPushButton(this);
};

QCameraWindow::QCameraWindow(QWidget *parent)
    : QWidget(parent), mContent(new QCameraWindowDialogContentImpl), mImageView(new QImageView(1280, 720))
{
    initDialog();
}

QCameraWindow::~QCameraWindow() noexcept {
    delete mContent;
}


void QCameraWindow::initDialog() {
    auto widgetGroup = new QWidget;
    widgetGroup->setLayout(mDialogLayout);
    mDialogLayout->addWidget(mConnectionsType);
    mDialogLayout->addWidget(mContent->videoSourceURL);
    mDialogLayout->addWidget(mContent->button);
    mWindowLayout->addWidget(widgetGroup);

    mConnectionsType->addItem(IMG_SOURCE_CVCAP);
    mConnectionsType->addItem("");
    mWindowLayout->addWidget(mImageView.get(), 1);


    QObject::connect(mConnectionsType, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
                     this, &QCameraWindow::comboBoxUpdate);

    QObject::connect(mContent->button, &QPushButton::clicked,
                     this, &QCameraWindow::cameraConnection);

}

void QCameraWindow::comboBoxUpdate(const QString &name) {
    std::cout << name.toStdString() << std::endl;
    if (name == IMG_SOURCE_CVCAP) {
        mContent->videoSourceURL->show();
    } else {
        mContent->videoSourceURL->hide();
    }
}

void QCameraWindow::cameraConnection() {
    if (mConnectionsType->currentText() == IMG_SOURCE_CVCAP) {
        if (mCamera == nullptr) {
            // TODO (ageev) заменить в интерфейсе std::string на QString
            //mCamera = std::make_unique<QContourHdCamera>();
            mCamera = std::make_unique<QCvVcCamera>(mContent->videoSourceURL->text().toStdString());
            QObject::connect(mCamera.get(), &QICamera::recvImage, mImageView.get(), &QImageView::updateImage);
        }


       // mCamera = std::make_unique<QContourHdCamera>();
        mCamera->play();
        mImageView->show();
        emit newCamera(mCamera);
    }

}

void QCameraWindow::initImageView() {

}

void QCameraWindow::closeEvent(QCloseEvent *event) {
    if (mCamera != nullptr) {
        mCamera->stop();
    }

    event->accept();
}


#include "QCameraWindow.moc"