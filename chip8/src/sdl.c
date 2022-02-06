#include "sdl.h"
#include "chip8_defines.h"

struct SDL_Struct sdl_init()
{
    struct SDL_Struct sdl;
    

    // retutns zero on success else non-zero
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }


    sdl.win = SDL_CreateWindow("GAME",
                                0,
                                0,
                                VIDEO_WIDTH * VIDEO_SCALE, 
                                VIDEO_HEIGHT * VIDEO_SCALE, 
                                SDL_WINDOW_SHOWN);

 
    // creates a renderer to render our images
    sdl.rend = SDL_CreateRenderer(sdl.win, -1, SDL_RENDERER_ACCELERATED);

    sdl.tex = SDL_CreateTexture(sdl.rend,
                                SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                VIDEO_WIDTH,
                                VIDEO_HEIGHT);

    return sdl;
}

void sdl_update_texture(SDL_Renderer* rend,  SDL_Texture* tex, void const* buffer, int pitch)
{
    SDL_UpdateTexture(tex, NULL, buffer, pitch);
    SDL_RenderClear(rend);
    SDL_RenderCopy(rend, tex, NULL, NULL);
    SDL_RenderPresent(rend);
}


uint8_t sdl_process_input(uint8_t* keys)
{
    uint8_t quit = 0;

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            {
                quit = 1;
            } break;

            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                    {
                        quit = 1;
                    } break;

                    case SDLK_x:
                    {
                        keys[0] = 1;
                    } break;

                    case SDLK_1:
                    {
                        keys[1] = 1;
                    } break;

                    case SDLK_2:
                    {
                        keys[2] = 1;
                    } break;

                    case SDLK_3:
                    {
                        keys[3] = 1;
                    } break;

                    case SDLK_q:
                    {
                        keys[4] = 1;
                    } break;

                    case SDLK_w:
                    {
                        keys[5] = 1;
                    } break;

                    case SDLK_e:
                    {
                        keys[6] = 1;
                    } break;

                    case SDLK_a:
                    {
                        keys[7] = 1;
                    } break;

                    case SDLK_s:
                    {
                        keys[8] = 1;
                    } break;

                    case SDLK_d:
                    {
                        keys[9] = 1;
                    } break;

                    case SDLK_z:
                    {
                        keys[0xA] = 1;
                    } break;

                    case SDLK_c:
                    {
                        keys[0xB] = 1;
                    } break;

                    case SDLK_4:
                    {
                        keys[0xC] = 1;
                    } break;

                    case SDLK_r:
                    {
                        keys[0xD] = 1;
                    } break;

                    case SDLK_f:
                    {
                        keys[0xE] = 1;
                    } break;

                    case SDLK_v:
                    {
                        keys[0xF] = 1;
                    } break;
                }
            } break;

            case SDL_KEYUP:
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_x:
                    {
                        keys[0] = 0;
                    } break;

                    case SDLK_1:
                    {
                        keys[1] = 0;
                    } break;

                    case SDLK_2:
                    {
                        keys[2] = 0;
                    } break;

                    case SDLK_3:
                    {
                        keys[3] = 0;
                    } break;

                    case SDLK_q:
                    {
                        keys[4] = 0;
                    } break;

                    case SDLK_w:
                    {
                        keys[5] = 0;
                    } break;

                    case SDLK_e:
                    {
                        keys[6] = 0;
                    } break;

                    case SDLK_a:
                    {
                        keys[7] = 0;
                    } break;

                    case SDLK_s:
                    {
                        keys[8] = 0;
                    } break;

                    case SDLK_d:
                    {
                        keys[9] = 0;
                    } break;

                    case SDLK_z:
                    {
                        keys[0xA] = 0;
                    } break;

                    case SDLK_c:
                    {
                        keys[0xB] = 0;
                    } break;

                    case SDLK_4:
                    {
                        keys[0xC] = 0;
                    } break;

                    case SDLK_r:
                    {
                        keys[0xD] = 0;
                    } break;

                    case SDLK_f:
                    {
                        keys[0xE] = 0;
                    } break;

                    case SDLK_v:
                    {
                        keys[0xF] = 0;
                    } break;
                }
            } break;
        }
    }

    return quit;
}

void sdl_destroy(struct SDL_Struct *sdl)
{
    // destroy texture
    SDL_DestroyTexture(sdl->tex);
 
    // destroy renderer
    SDL_DestroyRenderer(sdl->rend);
 
    // destroy window
    SDL_DestroyWindow(sdl->win);
     
    // close SDL
    SDL_Quit();
}