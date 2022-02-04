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
	printf("opcode = %x\n", op_code);
    printf("PC = %d\n", chip8->pc);

	chip8->pc+=2;
  

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


void set_pixel(SDL_Surface *surface, int x, int y, uint32_t pixel)
{
    uint32_t * const target_pixel = (uint32_t *) ((uint8_t *) surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel);
    *target_pixel = pixel;
}

void update_pixels(struct CHIP8 *chip8, SDL_Surface *surface)
{
    for(uint16_t y = 0; y < VIDEO_HEIGHT; y++)
    {
        for(uint16_t x = 0; x < VIDEO_WIDTH; x++)
        {
            set_pixel(surface, x, y, chip8->video[y * VIDEO_WIDTH + x ]);
        }
    }
}

int main(int argc, char *argv[])
{
	struct CHIP8 chip8;
	uint16_t op_code;
	
	init_chip8(&chip8);

	// Load given ROM into memory area
	load_rom(argv[1], &chip8);
	
    uint16_t counter = 0;

	
	for(uint16_t i = 0; i < 32; i++)
	{
		op_code = fetch(&chip8);
        print_register(&chip8);
		decode_execute(&chip8, op_code);
        print_video_memory(&chip8);
        printf("\ncounter = %d\n", counter);
        counter++;
	}
	
    
	// retutns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window* win = SDL_CreateWindow("GAME",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       VIDEO_WIDTH * VIDEO_SCALE, VIDEO_HEIGHT * VIDEO_SCALE, 0);

	// triggers the program that controls
    // your graphics hardware and sets flags
    uint32_t render_flags = SDL_RENDERER_ACCELERATED;
 
    // creates a renderer to render our images
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
 
    // creates a surface to load an image into the main memory
    SDL_Surface* surface;
 
    // please provide a path for your image
    surface = IMG_Load("/home/manucar/Pictures/tree.png");
 
    // loads image to our graphics hardware memory.
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
 
    // clears main-memory
    SDL_FreeSurface(surface);
 
    // let us control our image position
    // so that we can move it with our keyboard.
    SDL_Rect dest;
 
    // connects our texture with dest to control position
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
 
    // adjust height and width of our image box.
    dest.w /= 6;
    dest.h /= 6;
 
    // sets initial x-position of object
    dest.x = (1000 - dest.w) / 2;
 
    // sets initial y-position of object
    dest.y = (1000 - dest.h) / 2;
 
    // controls annimation loop
    int close = 0;
 
    // speed of box
    int speed = 1000;
    

    // annimation loop
    while (!close) {
        SDL_Event event;

        //update_pixels(&chip8, surface);
 
        // Events management
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
 
            case SDL_QUIT:
                // handling of close button
                close = 1;
                break;
 
            case SDL_KEYDOWN:
                // keyboard API for key pressed
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    dest.y -= speed / 30;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    dest.x -= speed / 30;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    dest.y += speed / 30;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    dest.x += speed / 30;
                    break;
                default:
                    break;
                }
            }
        }
 
        // right boundary
        if (dest.x + dest.w > 1000)
            dest.x = 1000 - dest.w;
 
        // left boundary
        if (dest.x < 0)
            dest.x = 0;
 
        // bottom boundary
        if (dest.y + dest.h > 1000)
            dest.y = 1000 - dest.h;
 
        // upper boundary
        if (dest.y < 0)
            dest.y = 0;
 
        // clears the screen
        SDL_RenderClear(rend);
        SDL_RenderCopy(rend, tex, NULL, &dest);
 
        // triggers the double buffers
        // for multiple rendering
        SDL_RenderPresent(rend);
 
        // calculates to 60 fps
        SDL_Delay(1000 / 60);
    }
 
    // destroy texture
    SDL_DestroyTexture(tex);
 
    // destroy renderer
    SDL_DestroyRenderer(rend);
 
    // destroy window
    SDL_DestroyWindow(win);
     
    // close SDL
    SDL_Quit();

    
 
    return 0;
	
}
