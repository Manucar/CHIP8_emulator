#ifndef __CHIP8_DEFINES__
#define __CHIP8_DEFINES__
#include <stdint.h>
#include <string.h>

#define START_ADDRESS 0x200
#define FONTSET_START_ADDRESS 0x50

#define FONTSET_SIZE  80

// CHIP8 Structure
struct CHIP8 {
	uint8_t registers[16];
	uint8_t memory[4096];
	uint16_t index;
	uint16_t pc;
	uint16_t stack[16];
	uint8_t sp;
	uint8_t delayTimer;
	uint8_t soundTimer;
	uint8_t keypad[16];
	uint32_t video[64 * 32];
	uint16_t opcode;
};


#endif