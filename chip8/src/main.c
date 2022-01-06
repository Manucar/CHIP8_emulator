#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "chip8_defines.h"

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

void init_chip8(struct CHIP8 *chip8){
	// Init PC to first usable address
	chip8->pc = START_ADDRESS;

	// Load fonts into memory
	for (uint8_t i = 0; i < FONTSET_SIZE; ++i)
	{
		chip8->memory[FONTSET_START_ADDRESS + i] = fontset[i];
	}

	// Init random number generator
	srand(time(NULL));

}

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

		printf("%x\n", chip8->memory[START_ADDRESS + bytes_read]);
		bytes_read++;
	}
	
	printf("bytes read = %d",bytes_read);
	
	fclose(fptr);
}

int main(int argc, char *argv[])
{
	struct CHIP8 chip8;
	
	init_chip8(&chip8);

	// Load given ROM in to memory block
	load_rom(argv[1], &chip8);
	
    return 0;
}
