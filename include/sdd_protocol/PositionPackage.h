//
// Created by Aleksey on 27.01.2020.
//

#ifndef SPPU_SERIAL_POSITION_H
#define SPPU_SERIAL_POSITION_H
#include "Package.h"

namespace sdd {
    /**
     * Пакет для установки задания на поизционирование рабочего органа
     */
    class PositionPackage : public Package{
    public:
        PositionPackage();
        explicit PositionPackage(std::vector<Package::byte_t> &bin_buff);

        /// Положение по оcи Ox
        /// Значения [-256; 255]
        short posX();
        void setPosX(short x);

        /// Положение по оcи Oy
        /// Значения [-256; 255]
        short posY();
        void setPosY(short x);
    private:
        void create_all_fields();
    };
}



#endif //SPPU_SERIAL_POSITION_H
