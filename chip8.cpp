#include "chip8.hpp"

unsigned char chip8Fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
    0x20, 0x60, 0x20, 0x20, 0x70,  // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
    0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
    0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
    0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
    0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
    0xF0, 0x80, 0xF0, 0x80, 0x80   // F
};

void Chip8::init() {
    // Program Counter starts at 0x200
    PC = 0x200;
    // reset current opcode
    opcode = 0;
    // reset Index Register
    I = 0;
    // reset Stack Pointer
    SP = 0;

    // clear display
    for (unsigned int i = 0; i < GFX_LEN; i++) {
        gfx[i] = 0;
    }
    // clear stack
    for (unsigned int i = 0; i < STACK_LEN; i++) {
        stack[i] = 0;
    }
    // clear registers V0-VF
    for (unsigned int i = 0; i < V_LEN; i++) {
        V[i] = 0;
    }
    // clear memory
    for (unsigned int i = 0; i < MEM_LEN; i++) {
        memory[i] = 0;
    }

    // load fontset into memory
    // (from memory location 0 to 80)
    for (unsigned int i = 0; i < 80; i++) {
        memory[i] = chip8Fontset[i];
    }

    // reset timers
    delayTimer = 0;
    soundTimer = 0;

    // TODO: random seed ?
}

Chip8::Chip8() {
    // CHIP-8 init
    init();
}

void Chip8::load() {}

void Chip8::emulateCycle() {}
