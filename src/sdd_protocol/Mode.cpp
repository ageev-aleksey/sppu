#include "sdd_protocol/Mode.h"

Mode::Mode() : Package("Mode", 20, Message::Endianness::MSG_LITTLE_ENDIAN)  {
    bworker.addField("C", 1);
}


Mode::Mode(std::vector<Package::byte_t> &buffer) : Package("Mode", 20, Message::Endianness::MSG_LITTLE_ENDIAN)  {
    bworker.addField("C", 1);
    bworker.fromBinary(buffer);
}


void Mode::coilOff() {
    mode_t m = COIL_OFF;
    findField("C")->fill(reinterpret_cast<const char*>(&m), 1);
}

void Mode::coilOn() {
    mode_t m = COIL_ON;
    findField("C")->fill(reinterpret_cast<const char*>(&m), 1);
}


Mode::mode_t Mode::getMode() {
    return findField("C")->to<Mode::mode_t>()[0];
}