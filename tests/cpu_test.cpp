#include "RegisterFile.hpp"
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include "memory.hpp"
#include "cpu.hpp"


TEST_CASE("8-bit Op-Codes :')") {
    CPU * cpu_t = new CPU();
    RegisterFile *rf_t1 = new RegisterFile();

    SECTION("FLAGS") {
        cpu_t->clearFlag(FLAG_C);
        REQUIRE(cpu_t->getFlag(FLAG_C) == false);
        cpu_t->setFlag(FLAG_C);
        REQUIRE(cpu_t->getFlag(FLAG_C) == true);
    }

    SECTION("Addition") {
        const uint8_t ADD_B = 0x80;
        rf_t1->writeReg(REG_A, 0x3A);
        rf_t1->writeReg(REG_B, 0xC6);

        cpu_t->setRegister(rf_t1);
        cpu_t->clearFlag(FLAG_Z);
        cpu_t->clearFlag(FLAG_H);
        cpu_t->clearFlag(FLAG_N);
        cpu_t->clearFlag(FLAG_C);

        cpu_t->execute(ADD_B);
        RegisterFile *rf = cpu_t->getRegister();

        REQUIRE(rf->readReg(REG_A, IS_8_BIT) == 0);
        REQUIRE(cpu_t->getFlag(FLAG_Z) == true);
        REQUIRE(cpu_t->getFlag(FLAG_H) == true);
        REQUIRE(cpu_t->getFlag(FLAG_N) == false);
        REQUIRE(cpu_t->getFlag(FLAG_C) == true);
    }

    SECTION("Subtraction") {
        const uint8_t SUB_B = 0x90;
        rf_t1->writeReg(REG_A, 0x3E);
        rf_t1->writeReg(REG_B, 0x3E);

        cpu_t->setRegister(rf_t1);
        cpu_t->clearFlag(FLAG_Z);
        cpu_t->clearFlag(FLAG_H);
        cpu_t->clearFlag(FLAG_N);
        cpu_t->clearFlag(FLAG_C);

        cpu_t->execute(SUB_B);
        RegisterFile *rf = cpu_t->getRegister();

        REQUIRE(rf->readReg(REG_A, IS_8_BIT) == 0);
        REQUIRE(cpu_t->getFlag(FLAG_Z) == true);
        REQUIRE(cpu_t->getFlag(FLAG_N) == true);
        REQUIRE(cpu_t->getFlag(FLAG_H) == false);
        REQUIRE(cpu_t->getFlag(FLAG_C) == false);

        rf_t1->writeReg(REG_A, 0x3E);
        rf_t1->writeReg(REG_B, 0x0F);

        cpu_t->setRegister(rf_t1);
        cpu_t->clearFlag(FLAG_Z);
        cpu_t->clearFlag(FLAG_H);
        cpu_t->clearFlag(FLAG_N);
        cpu_t->clearFlag(FLAG_C);

        cpu_t->execute(SUB_B);
        rf = cpu_t->getRegister();

        REQUIRE(rf->readReg(REG_A, IS_8_BIT) == 0x2F);
        REQUIRE(cpu_t->getFlag(FLAG_Z) == false);
        REQUIRE(cpu_t->getFlag(FLAG_N) == true);
        REQUIRE(cpu_t->getFlag(FLAG_H) == true);
        REQUIRE(cpu_t->getFlag(FLAG_C) == false);
    }
}
