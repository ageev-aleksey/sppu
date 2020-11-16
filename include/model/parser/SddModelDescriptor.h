//
// Created by nrx on 07.08.2020.
//

#ifndef TESTSIMULINKMODEL_SDDMODELDESCRIPTOR_H
#define TESTSIMULINKMODEL_SDDMODELDESCRIPTOR_H
#include "model/SddModel.h"
#include <vector>

struct TimedInput {
    SddModel::Input input;
    double time;
};

struct SddModelDescriptor {
    SddModel::Parameters parameters;
    std::vector<TimedInput> inputs;
    std::vector<SddModel::State> outputs;
};

#endif //TESTSIMULINKMODEL_SDDMODELDESCRIPTOR_H
