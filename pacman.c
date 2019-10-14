#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

#define SCREEN_W 224
#define SCREEN_H 288
#define DRAW_SCALE 3
#define SPRITE_SIZE 12
#define ALPHNUM_SIZE 6

typedef enum
{
    DIR_EAST,
    DIR_SOUTH,
    DIR_WEST,
    DIR_NORTH
} direction_t;


typedef struct
{
    int x, y;
    direction_t dir;
} player_t;



SDL_Rect GetFrame (int x, int y)
{
    SDL_Rect rect = { x*SPRITE_SIZE, y*SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE };
    return rect;
}

void PrintNumber (int num, int x, int y, SDL_Renderer * renderer, SDL_Texture * t)
{
    SDL_Rect src = { num * ALPHNUM_SIZE, 0, ALPHNUM_SIZE, ALPHNUM_SIZE };
    SDL_Rect dst = { x, y, ALPHNUM_SIZE, ALPHNUM_SIZE };
    SDL_RenderCopy(renderer, t, &src, &dst);
}

void DrawPlayer (SDL_Renderer * renderer, SDL_Texture * t, player_t * player)
{
    SDL_Rect src = GetFrame(player->dir, 3);
    SDL_Rect dst = { player->x, player->y, SPRITE_SIZE, SPRITE_SIZE };
    SDL_RenderCopy(renderer, t, &src, &dst);
}



int main ()
{
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Surface * temp;
    SDL_Texture * sprite_sheet;
    SDL_Event event;
    bool quit;
    player_t player;
    const uint8_t * kbstate = SDL_GetKeyboardState(NULL);

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

    player.x = 0;
    player.y = 0;
    player.dir = DIR_SOUTH;

    quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;
        }
        
        if (kbstate[SDL_SCANCODE_RIGHT]) {
            player.x++;
            player.dir = DIR_EAST;
        }
        if (kbstate[SDL_SCANCODE_UP]) {
            player.y--;
            player.dir = DIR_NORTH;
        }
        if (kbstate[SDL_SCANCODE_LEFT]) {
            player.x--;
            player.dir = DIR_WEST;
        }
        if (kbstate[SDL_SCANCODE_DOWN]) {
            player.y++;
            player.dir = DIR_SOUTH;
        }
        // draw
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        DrawPlayer(renderer, sprite_sheet, &player);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    // clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

	return 0;
}

