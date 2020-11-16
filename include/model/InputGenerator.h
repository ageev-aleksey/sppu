//
// Created by nrx on 14.08.2020.
//

#ifndef TESTSIMULINKMODEL_INPUTGENERATOR_H
#define TESTSIMULINKMODEL_INPUTGENERATOR_H
#include "model/SddModel.h"

/**
 * Интерфейс генератора входного сигнала в модель на каждом шаге моделирования.
 */
struct InputGenerator {
    /**
     * Генерация входа в модель.
     * @param state состояние sppu на текущем шаге моделирования
     * @return входной сигнал в модель Sppu
     */
    virtual SddModel::Input generate(const SddModel::State &state) = 0;
};

#endif //TESTSIMULINKMODEL_INPUTGENERATOR_H
