////
//// Created by Aleksey on 15.03.2020.
////

#include "sdd_protocol/connect/PackageBuffer.h"
#include "sdd_protocol/PackageError.h"
using namespace sdd::conn;

PackageBuffer::PackageBuffer(size_t size) : buffer(size) {
}

bool PackageBuffer::formPackage(sdd::StatePackage &state) {
	if (buffer.size() >= StatePackage::NUM_BYTES) {
        auto begin = buffer.begin();
        auto endPackage = begin + StatePackage::NUM_BYTES;
        auto endBuffer = buffer.end();
        StatePackage res;
        while(endPackage != endBuffer) {
            if (StatePackage::checkStruct(begin, endPackage)) {
                std::vector<char> tmp(begin, endPackage);
                state.fromBinary(tmp);
                buffer.erase_begin(std::distance(buffer.begin(), endPackage));
                return true;
            }
            ++begin;
            ++endPackage;
        }
	}
	return false;
}

void PackageBuffer::flush() {
    buffer.clear();
}
//
//std::ostream &operator<<(std::ostream &stream, const PackageBuffer &buff) {
//	for (const auto &el : buff.buffer) {
//stream << (int)el << "-";
//	}
//	return stream;
//}


