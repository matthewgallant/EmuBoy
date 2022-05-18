#include "RegisterFile.hpp"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

TEST_CASE("Write 8-Bit value to Register") {
    RegisterFile *rf = new RegisterFile();
    rf->writeReg(REG_A, 0x16);
    REQUIRE(rf->readReg(REG_A, IS_8_BIT) == 0x16);
    delete rf;
}

TEST_CASE("Write 16-Bit value to Register") {
    RegisterFile *rf = new RegisterFile();
    rf->writeReg(REG_AF, 0x1615, false);
    REQUIRE(rf->readReg(REG_AF, IS_16_BIT) == 0x1615);
    delete rf;
}


TEST_CASE("Update PC") {
    RegisterFile *rf = new RegisterFile();
    rf->setPC(0x666);
    REQUIRE(rf->getPC() == 0x666);
}

TEST_CASE("Update SP") {
    RegisterFile *rf = new RegisterFile();
    rf->setSP(0x666);
    REQUIRE(rf->getSP() == 0x666);
}
