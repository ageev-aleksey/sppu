#include "sdd_protocol/StatePackage.h"
using namespace sdd;

StatePackage::StatePackage() : Package("state", 20, Message::Endianness::MSG_LITTLE_ENDIAN) {
    create_all_fields();
}



StatePackage::StatePackage(const std::vector<Package::byte_t> &bin_buff) : Package("state", ID, Message::Endianness::MSG_LITTLE_ENDIAN) {
    create_all_fields();
    fromBinary(bin_buff);
}

StatePackage::~StatePackage() {

}

/*
void StatePackage::fromBinary(std::vector<Package::byte_t> bin_buff) {//TODO копирование
   if((bin_buff.size() == 36) && (bin_buff[0] == id())) {
        bworker.fromBinary(bin_buff, 1); //TODO копирование
   }

}*/

Package::byte_t StatePackage::state() {
    return findField("TW")->to<Package::byte_t>()[0];
}

void StatePackage::setState(Package::byte_t s) {
    findField("TW")->fill(&s, 1);
}

short StatePackage::OX() {
    return findField("OX")->to<short>()[0];
}

void StatePackage::setOX(short x) {
    findField("OX")->fill(reinterpret_cast<const char*>(&x), 2);
}

short StatePackage::OY() {
    return  findField("OY")->to<short>()[0];
}

void StatePackage::setOY(short y) {
    findField("OY")->fill(reinterpret_cast<const char*>(&y), 2);
}

short StatePackage::PWMX() {
    return findField("PWMX")->to<short>()[0];
}

void StatePackage::setPWMX(short pwmx){
    findField("PWMX")->fill(reinterpret_cast<const char*>(&pwmx), 2);
}

short StatePackage::PWMY() {
    return findField("PWMY")->to<short>()[0];
}

void StatePackage::setPWMY(short pwmy){
    findField("PWMY")->fill(reinterpret_cast<const char*>(&pwmy), 2);
}

short StatePackage::positionX() {
    return findField("POSX")->to<short>()[0];
}

void  StatePackage::setPositionX(short pos) {
    findField("POSX")->fill(reinterpret_cast<const char*>(&pos), 2);
}

short StatePackage::positionY() {
    return findField("POSY")->to<short>()[0];
}

void  StatePackage::setPositionY(short pos) {
    findField("POSY")->fill(reinterpret_cast<const char*>(&pos), 2);
}

Package::byte_t StatePackage::randomValue() {
    return findField("R")->to<Package::byte_t>()[0];
}



void StatePackage::create_all_fields() {
	bworker.addField("TW", 1);
	bworker.addField("OX", 2);
	bworker.addField("OY", 2);
	bworker.addField("-1", 3);//Байты, значения которых не удалось определить
	bworker.addField("PWMX", 2);
	bworker.addField("PWMY", 2);
	bworker.addField("POSX", 2);
	bworker.addField("POSY", 2);
	bworker.addField("-2", 17);//Байты, значения которых не удалось определить
	bworker.addField("R", 1);
}


