#ifndef __CHIP8_DEFINES__
#define __CHIP8_DEFINES__

#include <stdint.h>
#include <stdlib.h>

#define START_ADDRESS 0x200
#define FONTSET_START_ADDRESS 0x50

#define FONTSET_SIZE  80
#define VIDEO_WIDTH 64
#define VIDEO_HEIGHT 32

#define VIDEO_SCALE 10

/**
 * @brief CHIP8 structure
 * 
 */
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
	uint32_t video[VIDEO_WIDTH * VIDEO_HEIGHT];
	uint16_t opcode;
};


#endif