#include <stdio.h>
#include <time.h>
#include <string.h>


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>


//#include "chip8_defines.h"
#include "op_codes.h"

// Font set required (5x4 pixel each)
uint8_t fontset[FONTSET_SIZE] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

/**
 * @brief Initialize main memory block of the chip8
 * 
 * @param chip8 
 */
void init_chip8(struct CHIP8 *chip8){
	// Init PC to first usable address
	chip8->pc = START_ADDRESS;

	// Load fonts into memory
	for (uint8_t i = 0; i < FONTSET_SIZE; ++i)
	{
		chip8->memory[FONTSET_START_ADDRESS + i] = fontset[i];
	}

	// Init blank screen
	memset(chip8->video, 0, sizeof(chip8->video));

	// Init random number generator
	srand(time(NULL));

}

/**
 * @brief Load the content of the loaded ROM in the memory area of the chip8.
 * 
 * @param rom_path 
 * @param chip8 
 */
void load_rom(char *rom_path, struct CHIP8 *chip8){

	FILE *fptr;
    printf("%s\n", rom_path);

    fptr = fopen(rom_path,"r");

    if(fptr == NULL)
    {
		printf("Invalid ROM path!");   	
		exit(1);             
    }
	
	uint8_t read_byte[1];
	size_t fred_ret = 1;
    
	int bytes_read = 0;

	// Read all binary from file
	for (;;)
	{
		fred_ret = fread(read_byte, 1, 1, fptr);

		// Check if end of file
		if (fred_ret == 0)
			break;

		// Start inserting to a valid memory area
		chip8->memory[START_ADDRESS + bytes_read] = read_byte[0];

		//printf("%x\n", chip8->memory[START_ADDRESS + bytes_read]);
		bytes_read++;
	}
	
	//printf("bytes read = %d",bytes_read);
	
	fclose(fptr);
}


uint16_t fetch(struct CHIP8 *chip8)
{
	uint8_t first_byte = chip8->memory[chip8->pc];
	uint8_t second_byte = chip8->memory[chip8->pc + 1];
	uint16_t op_code = (uint16_t) (first_byte << 8 | second_byte);
	printf("%x\n", op_code);

	chip8->pc+=2;
	return op_code;
}


void decode_execute(struct CHIP8 *chip8, uint16_t op_code)
{
	OP_MSB_switch(chip8, op_code);
}


int main(int argc, char *argv[])
{
	struct CHIP8 chip8;
	uint16_t op_code;
	
	init_chip8(&chip8);

	// Load given ROM into memory area
	load_rom(argv[1], &chip8);
	
	/*
	for(uint16_t i = 0; i < 10; i++)
	{
		op_code = fetch(&chip8);
		decode_execute(&chip8, op_code);
	}
	*/


	// retutns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window* win = SDL_CreateWindow("GAME",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       1000, 1000, 0);

	for(;;)
	{

	}

    return 0;
}
