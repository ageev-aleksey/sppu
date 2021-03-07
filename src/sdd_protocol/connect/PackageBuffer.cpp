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
      //  std::cout << "is end: " << (endBuffer == endPackage) << std::endl;
        StatePackage res;
        bool isContinue = true;
        bool isEndStep = endPackage == endBuffer;
        while(isContinue) {
            if (StatePackage::checkStruct(begin, endPackage)) {
                std::vector<char> tmp(begin, endPackage);
                state.fromBinary(tmp);
                buffer.erase_begin(std::distance(buffer.begin(), endPackage));
                return true;
            }

            if (!isEndStep) {
                ++begin;
                ++endPackage;
                if (endPackage == endBuffer) {
                    isEndStep = true;
                }
            } else {
                isContinue = false;
            }

        }
	}
	return false;
}

void PackageBuffer::flush() {
    buffer.clear();
}

void PackageBuffer::toStream(std::ostream &stream) {

    for (const auto &el : buffer) {
        stream  << uint32_t {(unsigned char) el} << "-";
    }
    stream << std::endl;
}


