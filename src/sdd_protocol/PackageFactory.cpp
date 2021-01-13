#include "sdd_protocol/PackageFactory.h"
#include "sdd_protocol/State.h"
#include "sdd_protocol/PackageError.h"
#include <memory>


PackageFactory::PackageFactory(Message::Endianness endianness) {
	_endianness = endianness;
}

std::shared_ptr<Package> PackageFactory::createPackage(const std::vector<char> &bin_buffer) {
    if(bin_buffer.size() != 0) {
        switch(bin_buffer[0]) {
            case 20:
                return std::make_shared<State>(bin_buffer);
                break;
            default:
                throw PackageParseError("unknown package");
        }
    }
	throw PackageParseError("buffer must have a value");
}