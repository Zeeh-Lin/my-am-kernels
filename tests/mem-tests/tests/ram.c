#include "trap.h"

// Assume these addresses match your Linker Script and Hardware Memory Map
#define RAM_START 0x90000000 
#define RAM_SIZE  1024 // 1KB

void test_word_access() {
    volatile uint32_t *p = (uint32_t *)RAM_START;
    
    /* Test basic write and read back */
    p[0] = 0x12345678;
    p[1] = 0x87654321;
    
    check(p[0] == 0x12345678);
    check(p[1] == 0x87654321);
}

void test_byte_halfword_access() {
    volatile uint32_t *p = (uint32_t *)RAM_START;
    volatile uint8_t  *p8 = (uint8_t *)RAM_START;
    volatile uint16_t *p16 = (uint16_t *)RAM_START;

    /* Test byte access (Verifies dBus_cmd_payload_mask) */
    p[2] = 0x0; // Clear
    p8[8] = 0xAA;
    p8[9] = 0xBB;
    p8[10] = 0xCC;
    p8[11] = 0xDD;
    check(p[2] == 0xDDCCBBAA);

    /* Test half-word access */
    p[3] = 0x0; // Clear
    p16[6] = 0x55AA;      // Offset 12 bytes (index 6 for 16-bit)
    p16[7] = 0xFEED;      // Offset 14 bytes
    check(p[3] == 0xFEED55AA);
}

void test_memory_fill() {
    volatile uint32_t *p = (uint32_t *)RAM_START;
    uint32_t n = 64;

    /* Fill RAM with a pattern */
    for (uint32_t i = 0; i < n; i++) {
        p[i] = i ^ 0xAAAAAAAA;
    }

    /* Verify pattern */
    for (uint32_t i = 0; i < n; i++) {
        check(p[i] == (i ^ 0xAAAAAAAA));
    }
}

int main() {
    test_word_access();
    test_byte_halfword_access();
    test_memory_fill();

    /* If we reach here, all checks passed */
    halt(0); 
    return 0;
}