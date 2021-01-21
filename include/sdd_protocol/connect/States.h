//
// Created by nrx on 14.01.2021.
//

#ifndef TESTSIMULINKMODEL_STATE_H
#define TESTSIMULINKMODEL_STATE_H

#include <chrono>

namespace sdd::conn {
    struct Light {
        int blinking;
    };

    struct Mode {
        bool isOn;
    };

    struct TaskPosition {
        int ox;
        int oy;
    };
    struct Pwm {
        int ox;
        int oy;
    };
    struct State {
        int ox;
        int oy;
        Pwm pwm;
        TaskPosition task;
        std::chrono::steady_clock::time_point time;
    };
}
#endif //TESTSIMULINKMODEL_STATE_H
