//
// Created by Aleksey on 15.03.2020.
//

#ifndef SPPU_SERIAL_PACKAGEBUFFER_H
#define SPPU_SERIAL_PACKAGEBUFFER_H

#include "StatePackage.h"
#include <boost/circular_buffer.hpp>
#include <ostream>

namespace sdd {
    class PackageBuffer {
    public:
        explicit PackageBuffer(size_t size);
        void addBytes(const std::vector<unsigned char> &bytes);
        StatePackage formPackage();
        void flush();
        friend std::ostream &operator<<(std::ostream &stream, const PackageBuffer &buff);
//private:
        boost::circular_buffer<unsigned char> buffer;
    };
}
//TODO ����� ���������� ������� ��� ������ ���� ������� - StatePackage. /
// ���������� ����������, ����� ����� ������� ��� ����� �������. /
// ��� ����� ���������� ����������� ����� �������


std::ostream &operator<<(std::ostream &stream, const sdd::PackageBuffer &buff);

#endif //SPPU_SERIAL_PACKAGEBUFFER_H
