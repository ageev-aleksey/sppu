#include "gui/camera/QCameraWindow.h"
#include "camera/QRtspCamera.h"
#include "camera/QContourHDCamera.h"
#include <QLineEdit>
#include <QPushButton>

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
    mDialogLayout->addWidget(mConnectionsType);
    mDialogLayout->addWidget(mContent->videoSourceURL);
    mDialogLayout->addWidget(mContent->button);
    mConnectionsType->addItem(IMG_SOURCE_CVCAP);
    mConnectionsType->addItem("");

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
        // TODO (ageev) заменить в интерфейсе std::string на QString
       //mCamera = std::make_unique<QRtspCamera>(mContent->videoSourceURL->text().toStdString());
        mCamera = std::make_unique<QContourHdCamera>();

        
        QObject::connect(mCamera.get(), &QICamera::recvImage, mImageView.get(), &QImageView::updateImage);
        mImageView->show();
    }

}

void QCameraWindow::initImageView() {

}


#include "QCameraWindow.moc"