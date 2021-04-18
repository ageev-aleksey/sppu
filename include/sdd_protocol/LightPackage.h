//
// Created by Aleksey on 03.02.2020.
//

#ifndef SPPU_SERIAL_LIGHT_H
#define SPPU_SERIAL_LIGHT_H

#include "Package.h"
namespace sdd {
    /**
     * Пакеь управления светодиодом
     */
    class LightPackage : public Package {
    public:
        LightPackage();
        LightPackage(std::vector<Package::byte_t> &buff);

        /// Включить светодиод
        void lightOn();
        /// Выключить светодиод
        void lightOff();
        /// Мерцание светодиода
        void blinking(short n);
    };

}

#endif //SPPU_SERIAL_LIGHT_H
