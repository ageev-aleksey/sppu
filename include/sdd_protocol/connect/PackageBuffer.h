////
//// Created by Aleksey on 15.03.2020.
////

#ifndef SPPU_SERIAL_PACKAGEBUFFER_H
#define SPPU_SERIAL_PACKAGEBUFFER_H

#include "sdd_protocol/StatePackage.h"
#include <boost/circular_buffer.hpp>
#include <iostream>

namespace sdd::conn {
    class PackageBuffer {
    public:
        explicit PackageBuffer(size_t size);
        template <typename Iterator>
        void addBytes(const Iterator &begin, const Iterator &end) {
            buffer.insert(buffer.end(), begin, end);
        }
        bool formPackage(StatePackage &state);
        void flush();

private:
        //friend std::ostream &operator<<(std::ostream &stream, const sdd::conn::PackageBuffer &buff);
        boost::circular_buffer<char> buffer;
    };
}
////TODO ����� ���������� ������� ��� ������ ���� ������� - StatePackage. /
//// ���������� ����������, ����� ����� ������� ��� ����� �������. /
//// ��� ����� ���������� ����������� ����� �������


//std::ostream &operator<<(std::ostream &stream, const sdd::conn::PackageBuffer &buff);

#endif //SPPU_SERIAL_PACKAGEBUFFER_H
