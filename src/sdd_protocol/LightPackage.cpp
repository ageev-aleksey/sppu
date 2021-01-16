#include "sdd_protocol/LightPackage.h"
using namespace sdd;

LightPackage::LightPackage() : Package("light", 29, Message::Endianness::MSG_LITTLE_ENDIAN)
{
    bworker.addField("MW", 2);
}

LightPackage::LightPackage(std::vector<Package::byte_t> &buffer) : Package("light", 29, Message::Endianness::MSG_LITTLE_ENDIAN)
{
    bworker.addField("MW", 2);
    bworker.fromBinary(buffer);
}

void LightPackage::lightOff() {
    short value = 0;
    findField("MW")->fill(reinterpret_cast<const char*>(&value), 2);
}

void LightPackage::lightOn() {
    short value = -1;
    findField("MW")->fill(reinterpret_cast<const char*>(&value), 2);
}


void LightPackage::blinking(short n) {
    findField("MW")->fill(reinterpret_cast<const char*>(&n), 2);
}