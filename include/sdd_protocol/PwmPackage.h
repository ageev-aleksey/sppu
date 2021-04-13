//
// Created by nrx on 20.01.2021.
//

#ifndef SDDCLIENT_PWMPACKAGE_H
#define SDDCLIENT_PWMPACKAGE_H
#include "sdd_protocol/Package.h"
namespace sdd {
    /**
     * Пакет для установки значения скважности ШИМ
     */
    class PwmPackage : public Package {
    public:
        PwmPackage();
        void pwmOx(short pwm);
        short pwmOX();
        void pwmOy(short pwm);
        short pwmOy();

        void creatAllFields();
    };
} // namespace sdd

#endif //SDDCLIENT_PWMPACKAGE_H
