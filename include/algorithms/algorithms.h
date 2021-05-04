#ifndef SDDCLIENT_ALGORITHMS_H
#define SDDCLIENT_ALGORITHMS_H
#include <opencv2/core.hpp>

/**
 * Создание массива содержащий нули<br>
 * Например: <br>
 * При n = 5 получим массив: {0, 0, 0, 0, 0}
 * @param n длинна массива
 */
cv::Mat zeros(size_t n);

/**
 * Создание массива содержащего равномерно возрастающие
 * число начиная с единицы <br>
 * Например:<br>
 * При n = 5 получим массив: {1, 2, 3, 4, 5}
 * @param n
 * @return
 */
cv::Mat identity(size_t n);

/**
 * Поиск координат красной точки на изображении
 * @param image изображения, на котором вести поиск
 * @return координаты на изображении предполагаемой красной точки
 */
cv::Point findRedPointCoordinates(cv::Mat &image);

#endif //SDDCLIENT_ALGORITHMS_H
