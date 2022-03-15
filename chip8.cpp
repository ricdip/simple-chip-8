#include "chip8.hpp"

#include <fstream>
#include <stdexcept>

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
        // (we start filling the memory from location 0x200 = 512)
        for (unsigned int i = 0; i < size; i++) {
            memory[i + 512] = fileContent[i];
        }

        delete[] fileContent;
    } else {
        throw std::runtime_error("Unable to open file");
    }
}

void Chip8::emulateCycle() {
    // fetch opcode
    // an opcode is 2 bytes long
    // in order to merge both bytes, we have to:
    //  - fetch the first byte (at location PC)
    //  - shift it 8 bit to the left (to make enough space for the second byte)
    //  - fetch the second byte (at location PC + 1 (the next byte))
    //  - use the bitwise OR operation to merge both bytes
    opcode = (memory[PC] << 8) | memory[PC + 1];

    // decode opcode
    // an opcode consist of 2 bytes = 4 nibbles (or half-bytes)
    // the first nibble divides CHIP-8 instructions into broad categories: so we
    // have to mask (bitwise AND) the first nibble in the instruction
    switch (opcode & 0xF000) {
        // 00E_: can be 00E0 or 00EE
        // we have to mask the last nibble to find out
        case 0x0000:
            switch (opcode & 0x000F) {
                // 00E0: clear screen
                case 0x0000:
                    for (unsigned int i = 0; i < GFX_LEN; i++) {
                        gfx[i] = 0;
                    }
                    drawFlag = true;
                    PC += 2;
                    break;

                // 00EE: return from subroutine
                case 0x000E:
                    SP--;
                    PC = stack[SP];
                    PC += 2;
                    break;
            }
            break;

        // 1NNN: jump
        case 0x1000:
            // we set PC to NNN, causing the program to jump
            // to that memory location. We do not increment PC afterwards
            PC = opcode & 0x0FFF;
            break;

        // 6XNN: set VX = NN
        case 0x6000:
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            PC += 2;
            break;

        // 7XNN: adds NN to VX (carry flag is not changed)
        case 0x7000:
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            PC += 2;
            break;

        // ANNN: sets I = NNN
        case 0xA000:
            I = opcode & 0x0FFF;
            PC += 2;
            break;

        // DXYN: display
        // Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels
        // and a height of N pixels. Each row of 8 pixels is read as bit-coded
        // starting from memory location I; I value doesn’t change after the
        // execution of this instruction. As described above, VF is set to 1 if
        // any screen pixels are flipped from set to unset when the sprite is
        // drawn, and to 0 if that doesn’t happen.
        case 0xD000: {
            // fetch position and height of the sprite
            unsigned short x = V[(opcode & 0x0F00) >> 8];
            unsigned short y = V[(opcode & 0x00F0) >> 4];
            unsigned short height = opcode & 0x000F;
            // pixel value
            unsigned short pixel;
            // reset register VF
            V[0xF] = 0;
            // loop over each row
            for (int yline = 0; yline < height; yline++) {
                // fetch the pixel value from the memory
                // starting at location I
                pixel = memory[I + yline];
                // loop over 8-bits of 1 row
                for (int xline = 0; xline < 8; xline++) {
                    // check if current evaluated pixel is set
                    // to 1 (`0x80 >> xline` scan through the byte, one bit at
                    // the time -> 0x80 = 10000000)
                    if ((pixel & (0x80 >> xline)) != 0) {
                        // check if the pixel on the display is set to 1. If it
                        // is set to 1, we need to register the collision by
                        // setting the VF register
                        if (gfx[x + xline + ((y + yline) * 64)] == 1) {
                            V[0xF] = 1;
                        }
                        // set the pixel value by using bitwise XOR
                        gfx[x + xline + ((y + yline) * 64)] ^= 1;
                    }
                }
            }

            // we changed gfx[], so we need to update the screen
            drawFlag = true;
            // update the Program Counter to move to the next opcode
            PC += 2;

        } break;

        default:
            throw std::runtime_error("Unknown opcode");
    }

    // update timers
    if (delayTimer > 0) {
        delayTimer--;
    }

    if (soundTimer > 0) {
        if (soundTimer == 1) {
            // TODO: sound beep
        }
        soundTimer--;
    }
}

const bool* Chip8::getGfx() const { return gfx; }
