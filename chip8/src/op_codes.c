#include "chip8_defines.h"

/**
 * @brief CLS - Clear the display
 * 
 * @param chip8 
 */
void chip8_CLS_00E0(struct CHIP8 *chip8)
{
	memset(chip8->video, 0, sizeof(chip8->video));
}

/**
 * @brief RET - Return from a subroutine.
 * 
 * @param chip8 
 */
void chip8_RET_00EE(struct CHIP8 *chip8)
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
void chip8_JP_1nnn(struct CHIP8 *chip8, uint16_t opcode)
{
	chip8->pc = opcode & 0x0FFF;
}

/**
 * @brief CALL - Call subrotine at 'nnn' memory location.
 * 
 * @param chip8 
 * @param opcode 
 */
void chip8_CALL_2nnn(struct CHIP8 *chip8, uint16_t opcode)
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
void chip8_SE_3xkk(struct CHIP8 *chip8, uint16_t opcode)
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
void chip8_SNE_4xkk(struct CHIP8 *chip8, uint16_t opcode)
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
void chip8_SE_5xy0(struct CHIP8 *chip8, uint16_t opcode)
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
void chip8_LD_6xkk(struct CHIP8 *chip8, uint16_t opcode)
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
void chip8_ADD_7xkk(struct CHIP8 *chip8, uint16_t opcode)
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
void chip8_LD_8xy0(struct CHIP8 *chip8, uint16_t opcode)
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
void chip8_OR_8xy1(struct CHIP8 *chip8, uint16_t opcode)
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
void chip8_AND_8xy2(struct CHIP8 *chip8, uint16_t opcode)
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
void chip8_XOR_8xy3(struct CHIP8 *chip8, uint16_t opcode)
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
void chip8_ADD_8xy4(struct CHIP8 *chip8, uint16_t opcode)
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
void chip8_SUB_8xy5(struct CHIP8 *chip8, uint16_t opcode)
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
void chip8_SHR_8xy6(struct CHIP8 *chip8, uint16_t opcode)
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
void chip8_SUBN_8xy7(struct CHIP8 *chip8, uint16_t opcode)
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
void chip8_SHL_8xyE(struct CHIP8 *chip8, uint16_t opcode)
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
void chip8_SNE_9xy0(struct CHIP8 *chip8, uint16_t opcode)
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
void chip8_LD_Annn(struct CHIP8 *chip8, uint16_t opcode)
{
	chip8->index = opcode & 0x0FFF;
}

 /**
 * @brief JP - Jump to location nnn + V0.
 * 
 * @param chip8 
 * @param opcode
 */
void chip8_JP_Bnnn(struct CHIP8 *chip8, uint16_t opcode)
{
	chip8->pc = (opcode & 0x0FFF) + chip8->registers[0x0];
}

 /**
 * @brief RND - Set Vx = random byte AND kk.
 * 
 * @param chip8 
 * @param opcode
 */
void chip8_RND_Cxkk(struct CHIP8 *chip8, uint16_t opcode)
{
	uint8_t Vx = (uint8_t)(opcode >> 8) & 0x0F;
	uint8_t kk = (uint8_t)(opcode & 0x00FF);

	chip8->registers[Vx] = (uint8_t)(rand() & 0xFF) & kk;
}
