#include "chip8_defines.h"

void chip8_00E0(struct CHIP8 *chip8)
{
	memset(chip8->video, 0, sizeof(chip8->video));
}

