//
// Created by Aleksey on 03.02.2020.
//

#ifndef SPPU_SERIAL_MODE_H
#define SPPU_SERIAL_MODE_H
#include "Package.h"

namespace sdd {
    /**
     * Пакет текущего режима работы
     */
    class ModePackage : public Package {
    public :
        enum mode_t : Package::byte_t {
            COIL_OFF = 1, COIL_ON = 56
        };
        ModePackage();
        ModePackage(std::vector<Package::byte_t> &buffer);

        /// Включить устройство
        void coilOn();
        /// Выключить устройство
        void coilOff();
        mode_t getMode();
    };
}




#endif //SPPU_SERIAL_MODE_H
