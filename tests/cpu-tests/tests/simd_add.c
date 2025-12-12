#include "trap.h"

uint32_t simd_add() {
    register uint32_t rs1 asm("x6") = 0x11223344;
    register uint32_t rs2 asm("x7") = 0x01010101;
    register uint32_t rd  asm("x5");

    asm volatile(
        ".insn r 0x33, 0, 0x03, %0, %1, %2"
        : "=r"(rd)
        : "r"(rs1), "r"(rs2)
    );

    return rd;
}

int main() {
  check(simd_add() == 0x12233445);
}