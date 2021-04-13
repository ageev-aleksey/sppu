#include "sdd_protocol/ModePackage.h"
using namespace sdd;

ModePackage::ModePackage() : Package("ModePackage", 20, Message::Endianness::MSG_LITTLE_ENDIAN)  {
    bworker.addField("C", 1);
}


ModePackage::ModePackage(std::vector<Package::byte_t> &buffer) : Package("ModePackage", 20, Message::Endianness::MSG_LITTLE_ENDIAN)  {
    bworker.addField("C", 1);
    bworker.fromBinary(buffer);
}


void ModePackage::coilOff() {
    mode_t m = COIL_OFF;
    findField("C")->fill(reinterpret_cast<const char*>(&m), 1);
}

void ModePackage::coilOn() {
    mode_t m = COIL_ON;
    findField("C")->fill(reinterpret_cast<const char*>(&m), 1);
}


ModePackage::mode_t ModePackage::getMode() {
    return findField("C")->to<ModePackage::mode_t>()[0];
}