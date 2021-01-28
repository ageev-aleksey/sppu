#include "sdd_protocol/connect/PackageBuffer.h"
#include <gtest/gtest.h>


TEST(PackageBuffer, Test1) {
    sdd::conn::PackageBuffer buffer(sdd::StatePackage::NUM_BYTES*2);
    sdd::StatePackage package;
    package.setPositionY(255);
    package.setPositionX(-255);
    package.setPWMX(200);
    package.setPWMX(-200);
    auto bin = package.toBinary();

    std::vector<char> fakeBin = {0, 1, 2, 3, 4, sdd::StatePackage::ID, sdd::StatePackage::ID
                                 , 0, 0, 0, 0, sdd::StatePackage::ID};
    buffer.addBytes(fakeBin.begin(), fakeBin.end());
    buffer.addBytes(bin.begin(), bin.end());

    sdd::StatePackage newPack;
    ASSERT_TRUE(buffer.formPackage(newPack));
    auto newBin = newPack.toBinary();
    ASSERT_EQ(newBin, bin);
}