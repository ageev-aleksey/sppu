//
// Created by nrx on 14.08.2020.
//

#ifndef TESTSIMULINKMODEL_PWDINPUTGENERATOR_H
#define TESTSIMULINKMODEL_PWDINPUTGENERATOR_H
#include "model/InputGenerator.h"
#include <mutex>

/**
 * Класс реализующий генератор входа для модели СППУ в виде ШИМ
 */
class PwdInputGenerator : public InputGenerator {
public:
    /**
     *
     * @param frequency частота ШИМ в Герцах.
     * @param minOutput значение выхода при участке логического нуля.
     * @param maxOutput значение выхода при участке логической единицы.
     */
    PwdInputGenerator(size_t frequency, int minOutput, int maxOutput);
    /**
     * Установка значения скважности.
     * @param oxDc значение скважности по оси Ox. От 0 до 1.
     * @param ozDc значение скважности по оси Oz. От 0 до 1.
     */
    void setDutyCycle(double oxDc, double ozDc);
    /**
     * Генерация сигнала ШИМ по формуле:
     *
     * f(x)=a(\frac{x}{b}-floor\(\frac{x}{b})))
     *   \\
      *  \{f(t)+c<0\}
     * @param state
     * @return
     */
    SddModel::Input generate(const SddModel::State &state) override;
private:
    double mFrequency;
    int mMinValue;
    int mMaxValue;
    std::mutex mMutex;
    double mOxDutyCycle;
    double mOzDutyCycle;
};

#endif //TESTSIMULINKMODEL_PWDINPUTGENERATOR_H
