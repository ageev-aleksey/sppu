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
   // return findField("OX")->to<short>()[0];
    short s3 = findField("S3")->to<short>()[0];
    short s1 = findField("S1")->to<short>()[0];
    return s3 - s1;
}

void StatePackage::setOX(short x) {
    findField("OX")->fill(reinterpret_cast<const char*>(&x), 2);
}

short StatePackage::OY() {
    // return  findField("OY")->to<short>()[0];

    short s2 = findField("S2")->to<short>()[0];
    short s0 = findField("S0")->to<short>()[0];
    return s2 - s0;
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
   // return 0;
}

void  StatePackage::setPositionX(short pos) {
   // findField("POSX")->fill(reinterpret_cast<const char*>(&pos), 2);
}

short StatePackage::positionY() {
     return findField("POSY")->to<short>()[0];
   // return 0;
}

void  StatePackage::setPositionY(short pos) {
   // findField("POSY")->fill(reinterpret_cast<const char*>(&pos), 2);
}

Package::byte_t StatePackage::randomValue() {
    return findField("R")->to<Package::byte_t>()[0];
}


short StatePackage::S0() {
    return findField("S0")->to<short>()[0];
}

short StatePackage::S1() {
    return findField("S1")->to<short>()[0];
}

short StatePackage::S2() {
    return findField("S2")->to<short>()[0];
}

short StatePackage::S3() {
    return findField("S3")->to<short>()[0];
}


void StatePackage::create_all_fields() {
    // Корректный разбор пакета для устройсвта посылающая 36 байт
	//bworker.addField("TW", 1);
	//bworker.addField("OX", 2);
	//bworker.addField("OY", 2);
	//bworker.addField("-1", 3);//Байты, значения которых не удалось определить
	//bworker.addField("PWMX", 2);
	//bworker.addField("PWMY", 2);
	//bworker.addField("POSX", 2);
	//bworker.addField("POSY", 2);
	//bworker.addField("-2", 17);//Байты, значения которых не удалось определить
        
 //   bworker.addField("TW", 1);
 //   bworker.addField("OX", 2);
 //   bworker.addField("OY", 2);
 //   bworker.addField("-1", 3);//Байты, значения которых не удалось определить
 //   bworker.addField("PWMX-old", 2);// некорректное поле
 //   bworker.addField("PWMY-old", 3);// некорректное поле
 //   bworker.addField("PWMX", 2);
 //   bworker.addField("PWMY", 2);
 //   //bworker.addField("-2", 17);//Байты, значения которых не удалось определить
 //   bworker.addField("-2", 11);
	//bworker.addField("R", 1);

    // Разбор пакета устройства посылающая 31 байт
    bworker.addField("TW", 1);
    bworker.addField("S0", 2);
    bworker.addField("S1", 2);
    bworker.addField("S2", 2);
    bworker.addField("S3", 2);
    bworker.addField("VX", 2);
    bworker.addField("VY", 2);
    bworker.addField("PWMX", 2);
    bworker.addField("PWMY", 2);
    bworker.addField("POSX", 2);
    bworker.addField("POSY", 2);
    bworker.addField("-1", 7);
    bworker.addField("R", 1);

}

void StatePackage::setS0(short v) {
    findField("S0")->fill(reinterpret_cast<const char*>(&v), 2);
}

void StatePackage::setS1(short v) {
    findField("S1")->fill(reinterpret_cast<const char*>(&v), 2);
}

void StatePackage::setS2(short v) {
    findField("S2")->fill(reinterpret_cast<const char*>(&v), 2);
}

void StatePackage::setS3(short v) {
    findField("S3")->fill(reinterpret_cast<const char*>(&v), 2);
}


