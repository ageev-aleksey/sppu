#include "sdd_protocol/Light.h"

Light::Light() : Package("light", 29, Message::Endianness::MSG_LITTLE_ENDIAN)
{
    bworker.addField("MW", 2);
}

Light::Light(std::vector<Package::byte_t> &buffer) : Package("light", 29, Message::Endianness::MSG_LITTLE_ENDIAN)
{
    bworker.addField("MW", 2);
    bworker.fromBinary(buffer);
}

void Light::lightOff() {
    short value = 0;
    findField("MW")->fill(reinterpret_cast<const char*>(&value), 2);
}

void Light::lightOn() {
    short value = -1;
    findField("MW")->fill(reinterpret_cast<const char*>(&value), 2);
}


void Light::blinking(short n) {
    findField("MW")->fill(reinterpret_cast<const char*>(&n), 2);
}