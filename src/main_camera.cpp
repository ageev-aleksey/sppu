#include "camera/QRtspCamera.h"
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QFormLayout>
#include <QMouseEvent>

class QImageView : public QWidget {
    Q_OBJECT
public:
    QImageView(int width, int height, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        //setLayout(new QHBoxLayout);
        m_imgView->setFixedWidth(width);
        m_imgView->setFixedHeight(height);
        setMouseTracking(true);
    }

    void mouseMoveEvent(QMouseEvent *ev) override {
        emit mouseMove(ev->pos().x(), ev->pos().y());
    }
public slots:
    void updateImage(cv::Mat img) {
        //       std::cout << "print" << std::endl;
        QImage image((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
        m_imgView->setPixmap(QPixmap::fromImage(image));
    }
signals:
    void mouseMove(int x, int y);
private:
    QLabel *m_imgView = new QLabel(this);
};

class App : public QWidget {
    Q_OBJECT
public:
    App(QImageView *imgView) {
        imgView->setParent(this);
        m_imgView = imgView;
        m_layout->addWidget(m_imgView, 2);
        m_imgView->setFixedWidth(640);
        m_imgView->setFixedHeight(480);
        QObject::connect(imgView, &QImageView::mouseMove, this, &App::updateMousePosition);

        auto infoLayout = new QHBoxLayout(this);

        auto innerLayout = new QFormLayout(this);
        innerLayout->addRow(new QLabel("OX:"), xPosition);
        infoLayout->addLayout(innerLayout);

        innerLayout = new QFormLayout(this);
        innerLayout->addRow(new QLabel("OY:"), yPosition);
        infoLayout->addLayout(innerLayout);

        m_layout->addLayout(infoLayout);
    }
private slots:
    void updateMousePosition(int x, int y) {
        xPosition->setText(QString::number(x));
        yPosition->setText(QString::number(y));
    };
private:
    QImageView *m_imgView = nullptr; //new QImageView(this);
    QLabel *xPosition = new QLabel(this);
    QLabel *yPosition = new QLabel(this);
    QVBoxLayout *m_layout = new QVBoxLayout(this);
};

int main(int argc, char **argv) {
    QApplication core(argc, argv);
    auto factory = QCameraMaker::make();
    auto camera = factory->create("rtsp://192.168.1.176:5554/camera");
    auto view = new QImageView(640, 480, nullptr);
    QObject::connect(camera.get(), &QICamera::recvImage, view, &QImageView::updateImage);
    App app(view);
    app.show();
    int status = core.exec();
    return status;
}

#include "main_camera.moc"