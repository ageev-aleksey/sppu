#ifndef SDDCLIENT_QPOINTPOSITIONINSERTER_H
#define SDDCLIENT_QPOINTPOSITIONINSERTER_H

#include "gui/model_saver/QModelDataGetter.h"
#include "camera/QICamera.h"
#include <memory>
#include <mutex>


class QPointPositionInserter : public QModelDataGetter {
public:
    explicit QPointPositionInserter(std::shared_ptr<QModelDataGetter> dataGetter,
                                    std::shared_ptr<QICamera>  camera);


    std::shared_ptr<QModelDataGetter> getGetter();


    std::shared_ptr<QICamera> getCamera();

    SavingData getData();
    cv::Point getPoint();
public slots:
    void setGetter(std::shared_ptr<QModelDataGetter> dataGetter);
    void setCamera(std::shared_ptr<QICamera>  camera);
private slots:
    void addData(SavingData &data);
    void addImage(cv::Mat frame);

private:
    void makeAndSendLastData();

private:
    std::shared_ptr<QModelDataGetter> m_getter;
    QMetaObject::Connection m_getterConnection;

    std::shared_ptr<QICamera> m_camera;
    QMetaObject::Connection m_cameraConnection;

    std::mutex m_pointMutex;
    cv::Point m_lastPoint;

    std::mutex m_dataMutex;
    SavingData m_lastData;
};

#endif //SDDCLIENT_QPOINTPOSITIONINSERTER_H
