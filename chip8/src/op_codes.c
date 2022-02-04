#include "op_codes.h"
#include "chip8_defines.h"
#include <string.h>
#include <stdio.h>


/**
 * @brief CLS - Clear the display.
 * 
 * @param chip8 
 */
void OP_00E0(struct CHIP8 *chip8)
{
	memset(chip8->video, 0, sizeof(chip8->video));
}

/**
 * @brief RET - Return from a subroutine.
 * 
 * @param chip8 
 */
void OP_00EE(struct CHIP8 *chip8)
{
	chip8->sp--;
	chip8->pc = chip8->stack[chip8->sp];
}

/**
 * @brief JP - Jump to specified 'nnn' memory location.
 * 
 * @param chip8 
 * @param opcode 
 */
void OP_1nnn(struct CHIP8 *chip8, uint16_t opcode)
{
	chip8->pc = opcode & 0x0FFF;
}

/**
 * @brief CALL - Call subrotine at 'nnn' memory location.
 * 
 * @param chip8 
 * @param opcode 
 */
void OP_2nnn(struct CHIP8 *chip8, uint16_t opcode)
{
	chip8->stack[chip8->sp] = chip8->pc;
	chip8->sp++;
	chip8->pc = opcode & 0x0FFF;
}

 /**
 * @brief SE -  Skip next instruction if Vx = kk.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_3xkk(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t value = (uint8_t)(opcode & 0x00FF);
	uint8_t reg = (uint8_t)(opcode >> 8) & 0x0F;

	if (chip8->registers[reg] == value)
	{
		chip8->pc+=2;
	}
}

 /**
 * @brief SNE -  Skip next instruction if Vx != kk.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_4xkk(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t value = (uint8_t)(opcode & 0x00FF);
	uint8_t reg = (uint8_t)(opcode >> 8) & 0x0F;

	if (chip8->registers[reg] != value)
	{
		chip8->pc+=2;
	}
}

 /**
 * @brief SE - Skip next instruction if Vx = Vy.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_5xy0(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;
	uint8_t Vy = (uint8_t)(opcode & 0x00F0);

	if (chip8->registers[Vx] == chip8->registers[Vy])
	{
		chip8->pc+=2;
	}
}

 /**
 * @brief LD - Set Vx = kk.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_6xkk(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;
	uint8_t value = (uint8_t)(opcode & 0x00FF);

	chip8->registers[Vx] = value;
}

 /**
 * @brief ADD - Set Vx = Vx + kk.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_7xkk(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;
	uint8_t value = (uint8_t)(opcode & 0x00FF);

	chip8->registers[Vx] += value;
}

/**
 * @brief LD -  Set Vx = Vy.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_8xy0(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;
	uint8_t Vy = (uint8_t)(opcode & 0x00F0);

	chip8->registers[Vx] = chip8->registers[Vy];
}

/**
 * @brief OR -  Set Vx = Vx OR Vy.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_8xy1(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;
	uint8_t Vy = (uint8_t)(opcode & 0x00F0);

	chip8->registers[Vx] |= chip8->registers[Vy];
}

/**
 * @brief AND -  Set Vx = Vx AND Vy.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_8xy2(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;
	uint8_t Vy = (uint8_t)(opcode & 0x00F0);

	chip8->registers[Vx] &= chip8->registers[Vy];
}

/**
 * @brief XOR -  Set Vx = Vx XOR Vy.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_8xy3(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;
	uint8_t Vy = (uint8_t)(opcode & 0x00F0);

	chip8->registers[Vx] ^= chip8->registers[Vy];
}

/**
 * @brief ADD -  Set Vx = Vx + Vy, set VF = carry.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_8xy4(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;
	uint8_t Vy = (uint8_t)(opcode & 0x00F0);

	if ((chip8->registers[Vx] += chip8->registers[Vy]) > 255)
	{
		chip8->registers[0xF] = 1;
	}
	else
	{
		chip8->registers[0xF] = 0;
	}
}

/**
 * @brief SUB - Set Vx = Vx - Vy, set VF = NOT borrow.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_8xy5(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;
	uint8_t Vy = (uint8_t)(opcode & 0x00F0);

	if (chip8->registers[Vx] > chip8->registers[Vy])
	{
		chip8->registers[0xF] = 1;
	}
	else
	{
		chip8->registers[0xF] = 0;
	}
	chip8->registers[Vx] -= chip8->registers[Vy];
}

/**
 * @brief SHR - Set Vx = Vx SHR 1. (SHift Right)
 * 
 * @param chip8 
 * @param opcode
 */
void OP_8xy6(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;

	chip8->registers[0xF] = (chip8->registers[Vx] & 0x01);
	chip8->registers[Vx] >>= 1;
}

/**
 * @brief SUBN - Set Vx = Vy - Vx, set VF = NOT borrow.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_8xy7(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;
	uint8_t Vy = (uint8_t)(opcode & 0x00F0);

	if (chip8->registers[Vy] > chip8->registers[Vx])
	{
		chip8->registers[0xF] = 1;
	}
	else
	{
		chip8->registers[0xF] = 0;
	}
	chip8->registers[Vx] = chip8->registers[Vy] - chip8->registers[Vx];
}

/**
 * @brief SHL - Set Vx = Vx SHL 1. (SHift Left)
 * 
 * @param chip8 
 * @param opcode
 */
void OP_8xyE(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;

	chip8->registers[0xF] = (chip8->registers[Vx] & 0x80) >> 7;
	chip8->registers[Vx] <<= 1;
}

 /**
 * @brief SNE - Skip next instruction if Vx != Vy.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_9xy0(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;
	uint8_t Vy = (uint8_t)(opcode & 0x00F0);

	if (chip8->registers[Vx] != chip8->registers[Vy])
	{
		chip8->pc+=2;
	}
}

 /**
 * @brief LD -  Set register I = nnn.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_Annn(struct CHIP8 *chip8, uint16_t opcode)
{
	chip8->index = opcode & 0x0FFF;
}

 /**
 * @brief JP - Jump to location nnn + V0.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_Bnnn(struct CHIP8 *chip8, uint16_t opcode)
{
	chip8->pc = (opcode & 0x0FFF) + chip8->registers[0x0];
}

 /**
 * @brief RND - Set Vx = random byte AND kk.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_Cxkk(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;
	uint8_t kk = (uint8_t)(opcode & 0x00FF);

	chip8->registers[Vx] = (uint8_t)(rand() & 0xFF) & kk;
}

 /**
 * @brief DRW - Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_Dxyn(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	uint8_t n = opcode & 0x000Fu;

	uint8_t read_sprite;
	uint32_t pixel_pos;

	// Set VF register to 0 for no collision
	chip8->registers[0xF] = 0;

	// Loop over every byte read from memory
	for(uint8_t row = 0; row < n; row++)
	{
		read_sprite = chip8->memory[chip8->index + row];

		// Loop over every bit in the byte
		for(uint8_t col = 0; col < 8; col++)
		{
			// Vx and Vy initial coords are added with current row and col values retrieved from the current sprite
			// bytes and bit position, the modulo handles the position rollover on the screen
			pixel_pos = (((chip8->registers[Vy] + row) % VIDEO_HEIGHT) * VIDEO_WIDTH) + ((chip8->registers[Vx] + col) % VIDEO_WIDTH);

			// Check if the bit is 1 or 0 starting from the MSB
			if ((read_sprite >> (7 - col)) & 0x01)
			{
				// Check if the bit is already set -> collision
				if (chip8->video[pixel_pos] == 0xFFFFFFFF)
				{
					chip8->registers[0xF] = 1;
				}

				// XOR the video pixel with the 32 bit mask, 
				chip8->video[pixel_pos] ^= 0xFFFFFFFF;
			}
		}
	}
}

 /**
 * @brief SKP - Skip next instruction if key with the value of Vx is pressed.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_Ex9E(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;

	if (chip8->keypad[chip8->registers[Vx]])
	{
		chip8->pc+=2;
	}
}

/**
 * @brief SKNP - Skip next instruction if key with the value of Vx is not pressed.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_ExA1(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;

	if (!chip8->keypad[chip8->registers[Vx]])
	{
		chip8->pc+=2;
	}
}

/**
 * @brief LD - Set Vx = delay timer value.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_Fx07(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;

	chip8->registers[Vx] = chip8->delayTimer;
}

/**
 * @brief LD - Wait for a key press, store the value of the key in Vx.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_Fx0A(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;

	if (chip8->keypad[0])
	{
		chip8->registers[Vx] = 0;
	}
	else if (chip8->keypad[1])
	{
		chip8->registers[Vx] = 1;
	}
	else if (chip8->keypad[2])
	{
		chip8->registers[Vx] = 2;
	}
	else if (chip8->keypad[3])
	{
		chip8->registers[Vx] = 3;
	}
	else if (chip8->keypad[4])
	{
		chip8->registers[Vx] = 4;
	}
	else if (chip8->keypad[5])
	{
		chip8->registers[Vx] = 5;
	}
	else if (chip8->keypad[6])
	{
		chip8->registers[Vx] = 6;
	}
	else if (chip8->keypad[7])
	{
		chip8->registers[Vx] = 7;
	}
	else if (chip8->keypad[8])
	{
		chip8->registers[Vx] = 8;
	}
	else if (chip8->keypad[9])
	{
		chip8->registers[Vx] = 9;
	}
	else if (chip8->keypad[10])
	{
		chip8->registers[Vx] = 10;
	}
	else if (chip8->keypad[11])
	{
		chip8->registers[Vx] = 11;
	}
	else if (chip8->keypad[12])
	{
		chip8->registers[Vx] = 12;
	}
	else if (chip8->keypad[13])
	{
		chip8->registers[Vx] = 13;
	}
	else if (chip8->keypad[14])
	{
		chip8->registers[Vx] = 14;
	}
	else if (chip8->keypad[15])
	{
		chip8->registers[Vx] = 15;
	}
	else
	{
		// Decrement the PC by 2 to point to this same instruction again
		chip8->pc -= 2;
	}
}

/**
 * @brief LD - Set delay timer = Vx.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_Fx15(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;

	chip8->delayTimer = chip8->registers[Vx];
}

/**
 * @brief LD - Set sound timer = Vx.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_Fx18(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;

	chip8->soundTimer = chip8->registers[Vx];
}

/**
 * @brief ADD - Set I = I + Vx.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_Fx1E(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;

	chip8->index += chip8->registers[Vx];
}

/**
 * @brief LD - Set I = location of sprite for digit Vx.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_Fx29(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;

	chip8->index = FONTSET_START_ADDRESS + (chip8->registers[Vx] * 5);
}

/**
 * @brief LD - Store BCD representation of Vx in memory locations I, I+1, and I+2.
 * 
 * @param chip8 
 * @param opcode
 */
void OP_Fx33(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;
	uint8_t value = chip8->registers[Vx];

	// Ones-place
	chip8->memory[chip8->index + 2] = value % 10;
	value /= 10;

	// Tens-place
	chip8->memory[chip8->index + 1] = value % 10;
	value /= 10;

	// Hundreds-place
	chip8->memory[chip8->index] = value % 10;
}

/**
 * @brief LD - Store registers V0 through Vx in memory starting at location I.
 * @param chip8 
 * @param opcode
 */
void OP_Fx55(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;

	for (uint8_t reg = 0; reg < Vx; reg++)
	{
		chip8->memory[chip8->index + reg] = chip8->registers[reg];
	}
}

/**
 * @brief LD - Read registers V0 through Vx from memory starting at location I.
 * @param chip8 
 * @param opcode
 */
void OP_Fx65(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;

	for (uint8_t reg = 0; reg < Vx; reg++)
	{
		chip8->registers[reg] = chip8->memory[chip8->index + reg];
	}
}


/**
 * @brief Array of function pointers that works as a look up table to
 * speed up the opcode decoding instead of a big amount of switch statement.
 * 
 */
void (*opcode_MSB[])(struct CHIP8 *chip8, uint16_t opcode) = 
{
	OP_0_switch,
	OP_1nnn,
	OP_2nnn,
	OP_3xkk,
	OP_4xkk,
	OP_5xy0,
	OP_6xkk,
	OP_7xkk,
	OP_8_switch,
	OP_9xy0,
	OP_Annn,
	OP_Bnnn,
	OP_Cxkk,
	OP_Dxyn,
	OP_E_switch,
	OP_F_switch
};

/**
 * @brief Switch function to handle '0' MSB opcodes
 * 
 * @param chip8 
 * @param opcode 
 */
void OP_0_switch(struct CHIP8 *chip8, uint16_t opcode)
{
	switch (opcode & 0x000F)
	{
	case 0x0:
		OP_00E0(chip8);
		break;

	case 0xE:
		OP_00EE(chip8);
		break;
	
	default:
		break;
	}
}


/**
 * @brief Switch function to handle '8' MSB opcodes
 * 
 * @param chip8 
 * @param opcode 
 */
void OP_8_switch(struct CHIP8 *chip8, uint16_t opcode)
{
	switch (opcode & 0x000F)
	{
	case 0x0:
		OP_8xy0(chip8, opcode);
		break;

	case 0x1:
		OP_8xy1(chip8, opcode);
		break;

	case 0x2:
		OP_8xy2(chip8, opcode);
		break;

	case 0x3:
		OP_8xy3(chip8, opcode);
		break;

	case 0x4:
		OP_8xy4(chip8, opcode);
		break;

	case 0x5:
		OP_8xy5(chip8, opcode);
		break;

	case 0x6:
		OP_8xy6(chip8, opcode);
		break;

	case 0x7:
		OP_8xy7(chip8, opcode);
		break;

	case 0xE:
		OP_8xyE(chip8, opcode);
		break;

	default:
		break;
	}
}

/**
 * @brief Switch function to handle 'E' MSB opcodes
 * 
 * @param chip8 
 * @param opcode 
 */
void OP_E_switch(struct CHIP8 *chip8, uint16_t opcode)
{
	switch (opcode & 0x00FF)
	{
	case 0xA1:
		OP_ExA1(chip8, opcode);
		break;

	case 0x9E:
		OP_Ex9E(chip8, opcode);
		break;

	default:
		break;
	}
}

/**
 * @brief Switch function to handle 'F' MSB opcodes
 * 
 * @param chip8 
 * @param opcode 
 */
void OP_F_switch(struct CHIP8 *chip8, uint16_t opcode)
{
	switch (opcode & 0x00FF)
	{
	case 0x07:
		OP_Fx07(chip8, opcode);
		break;

	case 0x0A:
		OP_Fx0A(chip8, opcode);
		break;

	case 0x15:
		OP_Fx15(chip8, opcode);
		break;

	case 0x18:
		OP_Fx18(chip8, opcode);
		break;

	case 0x1E:
		OP_Fx1E(chip8, opcode);
		break;

	case 0x29:
		OP_Fx29(chip8, opcode);
		break;

	case 0x33:
		OP_Fx33(chip8, opcode);
		break;

	case 0x55:
		OP_Fx55(chip8, opcode);
		break;

	case 0x65:
		OP_Fx65(chip8, opcode);
		break;

	default:
		break;
	}
}

/**
 * @brief Switch function to handle different opcodes based on MSB.
 * 
 * @param chip8 
 * @param opcode 
 */
void OP_MSB_switch(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t MSB = (uint8_t)(opcode >> 12);
	(*opcode_MSB[MSB])(chip8, opcode);
}
