#include "mips.hpp"

const Register MipsRegister::zero = Register(0, "zero");

const Register MipsRegister::v0 = Register(2, "v0");
const Register MipsRegister::v1 = Register(3, "v1");

const Register MipsRegister::paramRegs[] = {
    Register(4, "a0"), Register(5, "a1"), Register(6, "a2"), Register(7, "a3")};

const Register MipsRegister::tmpRegs[] = {
    Register(8, "t0"),  Register(9, "t1"),  Register(10, "t2"),
    Register(11, "t3"), Register(12, "t4"), Register(13, "t5"),
    Register(14, "t6"), Register(15, "t7"), Register(16, "s0"),
    Register(17, "s1"), Register(18, "s2"), Register(19, "s3"),
    Register(20, "s4"), Register(21, "s5"), Register(22, "s6"),
    Register(23, "s7"), Register(24, "t8"), Register(25, "t9")};

/** use for temporary values which cannot use a temporary register and which
 * you know noone will mess with ! */
const Register MipsRegister::tempFP = Register(20, "s4");
const Register MipsRegister::tempSP = Register(21, "s5");
const Register MipsRegister::tempVal1 = Register(22, "s6");
const Register MipsRegister::tempVal2 = Register(23, "s7");

const Register MipsRegister::gp = Register(28, "gp");
const Register MipsRegister::sp = Register(29, "sp");
const Register MipsRegister::fp = Register(30, "fp");
const Register MipsRegister::ra = Register(31, "ra");
