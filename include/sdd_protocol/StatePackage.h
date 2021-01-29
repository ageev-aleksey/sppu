#ifndef _STATE_H_
#define _STATE_H_
#include "sdd_protocol/Package.h"
//Todo move constructor

namespace sdd {
    /**
     * Пакет состояние устройства.
     * x - горизонтальная ось
     * y - вертикальная ось
     */
    class StatePackage : public Package {
    public:
        inline const static int ID = 20;
        inline const static int NUM_BYTES = 36;
        StatePackage();
        explicit StatePackage(const std::vector<Package::byte_t> &bin_buff);
        //void fromBinary(std::vector<Package::byte_t> bin_buff);
        ~StatePackage() override;

        Package::byte_t state();
        void setState(Package::byte_t s);

        /// Угол поворота по оси Ox
        /// Значения [-255; 256]
        short OX();
        void  setOX(short x);

        /// Угол поворота по оси Oy
        /// Значения [-255; 256]
        short OY();
        void setOY(short y);

        /// Значение скважности шим на катушках по оси Ox
        /// Значения [-255; 256]
        short PWMX();
        void setPWMX(short pwmx);

        /// Значение скважности шим на катушках по оси Oy
        /// Значения [-255; 256]
        short PWMY();
        void setPWMY(short pwmy);

        /// Значение задания на позиционирование по оси Ox
        short positionX();
        void setPositionX(short pos);

        /// /// Значение задания на позиционирование по оси Oy
        short positionY();
        void setPositionY(short pos);

        Package::byte_t  randomValue();

        template <typename Iterator>
        static bool checkStruct(const Iterator &begin, const Iterator &end) {
            if (std::distance(begin, end) == NUM_BYTES && *begin == ID) {
                auto last = end - 1;
                auto payloadEnd = last - 2;
                return *last == hash(begin, payloadEnd);
            }
            return false;
        }

        // Package::byte_t getHash();

    private:
        void create_all_fields();


    };
}


#endif