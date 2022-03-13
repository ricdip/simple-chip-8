#ifndef CHIP8_HPP
#define CHIP8_HPP

#define MEM_LEN 4096
#define V_LEN 16
#define GFX_LEN 64 * 32
#define STACK_LEN 16
#define KEY_LEN 16

class Chip8 {
   private:
    // CHIP-8 has 35 opcodes that are all 2 bytes long
    // 16 bits = 2 bytes -> unsigned short
    unsigned short opcode;

    // CHIP-8 has 4K = 4096 bytes of RAM memory in total
    // the fontset should be loaded at memory locations 0-80 (0x00-0x50)
    // the program should be loaded at memory 512-onwards (0x200-onwards)
    // 8 bit = 1 bytes -> unsigned char
    unsigned char memory[MEM_LEN];

    // CHIP-8 has 15 8-bit general purpose CPU registers
    // named V0-VE.
    // The 16th register (VF) is used for 'carry flag'
    // every CPU register has 8 bit = 1 byte length
    // 8 bit = 1 bytes -> unsigned char
    unsigned char V[V_LEN];

    // CHIP-8 has one 16-bit Index register I that points
    // at locations in memory
    // 16 bits = 2 bytes -> unsigned short
    unsigned short I;

    // CHIP-8 has one 16-bit Program counter PC that points
    // at the current instruction in memory
    // 16 bits = 2 bytes -> unsigned short
    unsigned short PC;

    // CHIP-8 has a black and white graphics and the screen
    // has a total of 2048 pixels (64 x 32): we can implement
    // this with an array of booleans that holds the pixel state (1 or 0)
    bool gfx[GFX_LEN];

    // CHIP-8 has two 8-bit timer registers that count at 60Hz
    // when these registers are set with a value > 0, they
    // will count down until 0
    // 8 bit = 1 bytes -> unsigned char
    unsigned char delayTimer;
    // the sound timer functions like the delay timer but also
    // sends a beeping sound whenever it reaches 0
    unsigned char soundTimer;

    // CHIP-8 has a stack used to remember the current location
    // before a jump is performed
    // (CHIP-8 instruction set has opcodes that allow the
    // program to jump to a certain address or call a subroutine)
    // so, anytime we perform a jump or call a subroutine, we
    // store the PC in the stack before proceeding
    // the stack stores 16-bit addresses (16 bits = 2 bytes -> unsigned short)
    // and has 16 levels of stack. In order to remember which level
    // of the stack is used, we need to implement a stack pointer (SP)
    unsigned short stack[STACK_LEN];
    unsigned short SP;

    // CHIP-8 has a keypad that contains 16 keys labelled
    // from 0 to F (hexadecimal numbers) in a 4x4 grid
    // 8 bit = 1 bytes -> unsigned char (we can store
    // the current state of the key with only 1 byte)
    unsigned char key[KEY_LEN];

    // initialize CHIP-8 emulator:
    //  - set default value for PC (0x200)
    //  - reset opcode, I, SP
    //  - clear display (gfx), stack, registers (V0-VF), memory
    //  - load fontset into the memory
    //  - reset timers
    void init();

   public:
    // initialize CHIP-8 emulator (calls init function)
    Chip8();
    // loads the program into the memory
    void load(const char* programPath);
    // emulates one cycle (one opcode execution):
    //  - fetch, decode, execute opcode
    //  - update timers
    void emulateCycle();
};

#endif
