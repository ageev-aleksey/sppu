#include "gui/model_saver/QPointPositionInserter.h"
#include "algorithms/algorithms.h"

#include <chrono>

QPointPositionInserter::QPointPositionInserter(std::shared_ptr<QModelDataGetter> dataGetter,
                                               std::shared_ptr<QICamera> camera)
{
    m_index = 0;
    setGetter(std::move(dataGetter));
    setCamera(std::move(camera));
}

void QPointPositionInserter::addData(SavingData &data) {
    { // lock
        std::lock_guard<std::mutex> lock(m_dataMutex);
        m_lastData = data;
    } // end lock

    makeAndSendLastData();
}

void QPointPositionInserter::setGetter(std::shared_ptr<QModelDataGetter> dataGetter) {
    if (m_getterConnection) {
        QObject::disconnect(m_getterConnection);
    }
    m_getter = std::move(dataGetter);
    if (m_getter != nullptr) {
        m_getterConnection = QObject::connect(m_getter.get(), &QModelDataGetter::newData,
                                              this, &QPointPositionInserter::addData);
    }
}

std::shared_ptr<QModelDataGetter> QPointPositionInserter::getGetter() {
    return m_getter;
}

void QPointPositionInserter::setCamera(std::shared_ptr<QICamera> camera) {
    if (m_cameraConnection) {
        QObject::disconnect(m_cameraConnection);
    }
    m_camera = std::move(camera);
    if (m_camera != nullptr) {
        m_cameraConnection = QObject::connect(m_camera.get(), &QICamera::recvImage,
                                              this, &QPointPositionInserter::addImage);
    }

}

std::shared_ptr<QICamera> QPointPositionInserter::getCamera() {
    return m_camera;
}

void QPointPositionInserter::addImage(cv::Mat frame) {
    auto time = std::chrono::steady_clock::now();
    auto point = findRedPointCoordinates(frame);

    { // lock
        std::lock_guard<std::mutex> lock(m_pointMutex);
        m_lastPoint = point;
        m_time = time;
    } // end locks

    makeAndSendLastData();
}

cv::Point QPointPositionInserter::getPoint() {
    std::lock_guard<std::mutex> lock(m_pointMutex);
    return m_lastPoint;
}

SavingData QPointPositionInserter::getData() {
    std::lock_guard<std::mutex> lock(m_dataMutex);
    return m_lastData;
}

void QPointPositionInserter::makeAndSendLastData() {
    SavingData data  = getData();
    cv::Point  point = getPoint();
    data.additional["pointPositionIndex"] = std::to_string(m_index);
    m_index++;
    data.additional["timeAddedPointInformation"] = std::to_string(
            std::chrono::duration_cast<std::chrono::microseconds>(m_time.time_since_epoch()).count());
    data.additional["redPointOx"] = std::to_string(point.x);
    data.additional["redPointOy"] = std::to_string(point.y);
    emit newData(data);
}
