#include <stdio.h>
#include <SDL2/SDL.h>

#define SCREEN_W 224
#define SCREEN_H 288
#define DRAW_SCALE 3

int main ()
{
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Surface * temp;
    SDL_Texture * sprite_sheet;

    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        puts("SDL_Init failed!");
        return 1;
    }

    window = SDL_CreateWindow("Pac-man", 0, 0, SCREEN_W*DRAW_SCALE, SCREEN_H*DRAW_SCALE, 0);
    if (!window) {
        puts("SDL_CreateWindow failed!!!!");
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        puts("SDL_CreateRenderer failed!");
        return 1;
    }
    SDL_RenderSetScale(renderer, DRAW_SCALE, DRAW_SCALE);

    temp = SDL_LoadBMP("spritemap.bmp");
    if (!temp) {
        puts("Could not load spritemap.bmp!");
        return 1;
    }

    sprite_sheet = SDL_CreateTextureFromSurface(renderer, temp);
    if (!sprite_sheet) {
        puts("Could not create texture sprite_sheet");
        return 1;
    }

    SDL_PumpEvents();

    // draw
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, sprite_sheet, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_Delay(5000);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

	return 0;
}

