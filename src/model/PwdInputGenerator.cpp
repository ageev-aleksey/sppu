#include "model/PwdInputGenerator.h"

namespace  {

    double saw(double x, double frequency) {
        double value = x / frequency;
        int floor = static_cast<int>( x / frequency);
        return value - static_cast<double>(floor);
    }

    bool step(double x, double dutyCycle, double frequency) {
        return saw(x, frequency) > dutyCycle;
    }

}


PwdInputGenerator::PwdInputGenerator(size_t frequency, int minOutput, int maxOutput)
: mFrequency(1.0/frequency), mMinValue(minOutput), mMaxValue(maxOutput)
{}

void PwdInputGenerator::setDutyCycle(double oxDc, double ozDc) {
    std::lock_guard<std::mutex> lock(mMutex);
    mOxDutyCycle = oxDc;
    mOzDutyCycle = ozDc;
}

SddModel::Input PwdInputGenerator::generate(const SddModel::State &state) {
    std::lock_guard<std::mutex> lock(mMutex);

    SddModel::Input res{};
    if(step(state.time, mOxDutyCycle, mFrequency)) {
        res.pwmOx = mMaxValue;
    } else {
        res.pwmOx = mMinValue;
    }

    if(step(state.time, mOzDutyCycle, mFrequency)) {
        res.pwmOz = mMaxValue;
    } else {
        res.pwmOz = mMinValue;
    }
    return res;
}


