#include "RegisterFile.hpp"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

TEST_CASE("Write 8-Bit value to Register") {
    RegisterFile *rf = new RegisterFile();
    rf->writeReg(REG_A, 0x16);
    REQUIRE(rf->readReg(REG_A, IS_8_BIT) == 0x16);
}
