#pragma once

#include <cstdint>
#include <string>

// max memory = 4096
#define MEM_LEN 4096
// 16 registers
#define V_LEN 16
// graphics width
#define GFX_WIDTH 64
// graphics height
#define GFX_HEIGHT 32
// graphics len = width x height
#define GFX_LEN (GFX_WIDTH * GFX_HEIGHT)
// max stack size
#define STACK_LEN 16
// 16 keys
#define KEY_LEN 16

class Chip8 {
private:
  // CHIP-8 has 35 opcodes that are all 2 bytes long
  // 2 bytes = 16 bits
  uint16_t opcode;

  // CHIP-8 has 4K = 4096 bytes of RAM memory in total
  // the fontset should be loaded at memory locations 0-80 (0x00-0x50)
  // the program should be loaded at memory 512-onwards (0x200-onwards)
  // 1 bytes = 8 bits
  uint8_t memory[MEM_LEN];

  // CHIP-8 has 15 8-bit general purpose CPU registers
  // named V0-VE.
  // The 16th register (VF) is used for 'carry flag'
  // every CPU register has 8 bit = 1 byte length
  // 1 bytes = 8 bits
  uint8_t V[V_LEN];

  // CHIP-8 has one 16-bit Index register I that points
  // at locations in memory
  // 2 bytes = 16 bits
  uint16_t I;

  // CHIP-8 has one 16-bit Program counter PC that points
  // at the current instruction in memory
  // 2 bytes = 16 bits
  uint16_t PC;

  // CHIP-8 has a black and white graphics and the screen
  // has a total of 2048 pixels (64 x 32): we can implement
  // this with an array of booleans that holds the pixel state (1 or 0)
  bool gfx[GFX_LEN];

  // CHIP-8 has two 8-bit timer registers that count at 60Hz
  // when these registers are set with a value > 0, they
  // will count down until 0
  // 1 bytes = 8 bits
  uint8_t delayTimer;
  // the sound timer functions like the delay timer but also
  // sends a beeping sound whenever it reaches 0
  // 1 bytes = 8 bits
  uint8_t soundTimer;

  // CHIP-8 has a stack used to remember the current location
  // before a jump is performed
  // (CHIP-8 instruction set has opcodes that allow the
  // program to jump to a certain address or call a subroutine)
  // so, anytime we perform a jump or call a subroutine, we
  // store the PC in the stack before proceeding
  // the stack stores 16-bit addresses (2 bytes = 16 bits)
  // and has 16 levels of stack. In order to remember which level
  // of the stack is used, we need to implement a stack pointer (SP)
  uint16_t stack[STACK_LEN];
  uint16_t SP;

  // CHIP-8 has a keypad that contains 16 keys labelled
  // from 0 to F (hexadecimal numbers) in a 4x4 grid
  // 1 bytes = 8 bits (we can store
  // the current state of the key with only 1 byte)
  uint8_t key[KEY_LEN];

  // initialize CHIP-8 emulator:
  //  - set default value for PC (0x200)
  //  - reset opcode, I, SP
  //  - clear display (gfx), stack, registers (V0-VF), memory
  //  - load fontset into the memory
  //  - reset timers
  void init();

public:
  // if drawFlag is true, redraw the screen
  bool drawFlag = false;
  // initialize CHIP-8 emulator (calls init function)
  Chip8();
  // loads the program into the memory
  void load(const std::string);
  // emulates one cycle (one opcode execution):
  //  - fetch, decode, execute opcode
  //  - update timers
  void emulateCycle();

  // returns graphics array for displaying
  // purposes
  const bool *getGfx() const;
};