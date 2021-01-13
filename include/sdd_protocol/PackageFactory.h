#ifndef SPPU_SERIAL_PACKAGEFACTORY_H
#define SPPU_SERIAL_PACKAGEFACTORY_H
#include "Package.h"
#include <memory>
class PackageFactory {
public:
    PackageFactory(Message::Endianness endianness = Message::Endianness::MSG_LITTLE_ENDIAN);
    std::shared_ptr<Package> createPackage(const std::vector<char> &bin_buffer);
private:
	Message::Endianness _endianness;
};



#endif //SPPU_SERIAL_PACKAGEFACTORY_H
