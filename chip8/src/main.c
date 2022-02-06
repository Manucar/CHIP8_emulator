#include <stdio.h>
#include <time.h>
#include <string.h>


//#include "chip8_defines.h"
#include "op_codes.h"
#include "sdl.h"

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

    // Init memory to 0
	memset(chip8->memory, 0, sizeof(chip8->memory));

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

		bytes_read++;
	}	
	fclose(fptr);
}


uint16_t fetch(struct CHIP8 *chip8)
{
	uint8_t first_byte = chip8->memory[chip8->pc];
	uint8_t second_byte = chip8->memory[chip8->pc + 1];
	uint16_t op_code = (uint16_t) (first_byte << 8 | second_byte);
	//printf("opcode = %x\n", op_code);
    //printf("PC = %d\n", chip8->pc);

    

	chip8->pc+=2;

    // Decrement the delay timer if it's been set
	if (chip8->delayTimer > 0)
	{
		--chip8->delayTimer;
	}

	// Decrement the sound timer if it's been set
	if (chip8->soundTimer > 0)
	{
		--chip8->soundTimer;
	}
  
	return op_code;
}


void decode_execute(struct CHIP8 *chip8, uint16_t op_code)
{
	OP_MSB_switch(chip8, op_code);
}


void print_video_memory(struct CHIP8 *chip8)
{
    for(uint16_t y = 0; y < VIDEO_HEIGHT; y++)
    {
        for(uint16_t x = 0; x < VIDEO_WIDTH; x++)
        {
            printf("%d ", chip8->video[y * VIDEO_WIDTH + x ]);
        }
        printf("\n");
    }
}

void print_register(struct CHIP8 *chip8)
{
    for(uint16_t j = 0; j < 16; j++)
    {
        printf("reg: %d, val: 0x%x \n", j, chip8->registers[j]);
    }
    printf("index: 0x%x \n", chip8->index);
}



int main(int argc, char *argv[])
{
	struct CHIP8 chip8;
	uint16_t op_code;
	
	init_chip8(&chip8);

	// Load given ROM into memory area
	load_rom(argv[1], &chip8);
	
	
	struct SDL_Struct sdl = sdl_init();
    
    uint16_t video_pitch = sizeof(chip8.video[0]) * VIDEO_WIDTH;
    uint8_t quit = 0;
    while (!quit)
	{

        op_code = fetch(&chip8);
        //print_register(&chip8);
		decode_execute(&chip8, op_code);
        //print_video_memory(&chip8);
        
		quit = sdl_process_input(chip8.keypad);

        sdl_update_texture(sdl.rend, sdl.tex, chip8.video, video_pitch);
 
        // calculates to 60 fps
        //SDL_Delay(1000 / 60);
    }
    
    sdl_destroy(&sdl);

    return 0;
	
}
