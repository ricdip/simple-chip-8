#include "chip8.hpp"

#include <fstream>

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
    // CHIP-8 initialization
    init();
}

void Chip8::load(const char* programPath) {
    std::ifstream file;
    std::streampos size;
    char* fileContent;

    // opening program ROM
    // mode:
    //  - ios::in     -> open for input operations
    //  - ios::binary -> open in binary mode
    //  - ios::ate    -> set initial position at the end of file
    file.open(programPath, std::ios::in | std::ios::binary | std::ios::ate);

    // opening and reading program ROM
    if (file.is_open()) {
        // tellg: returns current position of element to be read (in this case,
        // the position is at the end of file, so is the size of the file
        // that we opened)
        size = file.tellg();
        fileContent = new char[size];
        // seekg: we change the location of get position
        // (we set the pointer at the beginning of
        // the file)
        // seekg(offset, direction) with:
        //  - offset = 0
        //  - direction = ios::beg -> beginning of stream
        file.seekg(0, std::ios::beg);
        // reading program ROM file
        file.read(fileContent, size);
        // closing program ROM
        file.close();

        // loading program ROM into memory
        for (unsigned int i = 0; i < size; i++) {
            memory[i + 512] = fileContent[i];
        }

        delete[] fileContent;
    } else {
        throw "Unable to open file";
    }
}

void Chip8::emulateCycle() {}
