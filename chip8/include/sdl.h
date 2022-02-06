#ifndef __SDL__
#define __SDL__


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

/**
 * @brief SDL structure
 * 
 */
struct SDL_Struct {
    SDL_Window* win;
    SDL_Renderer* rend;
    SDL_Texture* tex;

};

struct SDL_Struct sdl_init();
void sdl_update_texture(SDL_Renderer* rend,  SDL_Texture* tex, void const* buffer, int pitch);
uint8_t sdl_process_input(uint8_t* keys);
void sdl_destroy(struct SDL_Struct *sdl);


#endif