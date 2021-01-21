#include "sdd_protocol/PwmPackage.h"

void sdd::PwmPackage::pwmOx(short pwm) {
    findField("PwmX")->fill(reinterpret_cast<const char*>(&pwm), 2);
}

void sdd::PwmPackage::pwmOy(short pwm) {
    findField("PwmY")->fill(reinterpret_cast<const char*>(&pwm), 2);
}

sdd::PwmPackage::PwmPackage()
    : Package("PwmControl", 21, Message::Endianness::MSG_LITTLE_ENDIAN)
{
    creatAllFields();
}

void sdd::PwmPackage::creatAllFields() {
    bworker.addField("PwmX", 2);
    bworker.addField("PwmY", 2);
}

short sdd::PwmPackage::pwmOX() {
    return  findField("PwmX")->to<short>()[0];
}

short sdd::PwmPackage::pwmOy() {
    return findField("PwmY")->to<short>()[0];
}
