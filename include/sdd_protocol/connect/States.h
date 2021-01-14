//
// Created by nrx on 14.01.2021.
//

#ifndef TESTSIMULINKMODEL_STATE_H
#define TESTSIMULINKMODEL_STATE_H
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

    struct State {
        int ox;
        int oy;
        int pwmX;
        int pwmY;
        TaskPosition task;
    };
}
#endif //TESTSIMULINKMODEL_STATE_H
