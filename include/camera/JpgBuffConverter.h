#ifndef SDDCLIENT_JPGBUFFCONVERTER_H
#define SDDCLIENT_JPGBUFFCONVERTER_H
#include <memory>
#include <vector>
#include <opencv2/core/mat.hpp>

/// Скрытая реализация
class JpgBuffConverterImpl;

/**
 * Калсс выполняющий декомпрессию jpg изображения
 * из буффера в cv::Mat
 */
class JpgBuffConverter {
public:
    JpgBuffConverter();
    ~JpgBuffConverter();
    /**
     * Декомпрессия jpg изображения
     * @param jpg_img - буффер с jpg изображением
     * @return Матрица изображения
     */
    cv::Mat operator()(const std::vector<unsigned char> &jpg_img);
private:
    JpgBuffConverterImpl *m_pimpl;
};

#endif //SDDCLIENT_JPGBUFFCONVERTER_H
