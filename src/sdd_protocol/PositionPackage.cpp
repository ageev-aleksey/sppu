
#include "sdd_protocol/PositionPackage.h"

using namespace sdd;
void PositionPackage::create_all_fields() {
    bworker.addField("POSX", 2);
    bworker.addField("POSY", 2);
}


PositionPackage::PositionPackage() : Package("position", 22, Message::Endianness::MSG_LITTLE_ENDIAN) {
    create_all_fields();
}

PositionPackage::PositionPackage(std::vector<Package::byte_t > &buffer)
: Package("position", 22, Message::Endianness::MSG_LITTLE_ENDIAN){
    create_all_fields();
    fromBinary(buffer);
}

short PositionPackage::posX(){
    return findField("POSX")->to<short>()[0];
}

void PositionPackage::setPosX(short x) {
    findField("POSX")->fill(reinterpret_cast<const char*>(&x), 2);
}


short PositionPackage::posY(){
    return findField("POSY")->to<short>()[0];
}

void PositionPackage::setPosY(short x) {
    findField("POSY")->fill(reinterpret_cast<const char*>(&x), 2);
}

