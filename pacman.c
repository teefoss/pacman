#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

#define SCREEN_W 224
#define SCREEN_H 288
#define DRAW_SCALE 3
#define SPRITE_SIZE 12
#define WALL_SPRITE_SIZE 6
#define ALPHNUM_SIZE 6
#define GAME_BOARD_TILES_HIGH 31
#define GAME_BOARD_TILES_WIDE 28

typedef enum
{
    DIR_EAST,
    DIR_SOUTH,
    DIR_WEST,
    DIR_NORTH
} direction_t;

typedef enum
{
     EMPTY,
     DOUGHT,
     FRUIT_SNAX,
     SICK_WALL_BRO,
} tile_type_t;

typedef struct
{
    int x, y;
    direction_t dir;
} player_t;

typedef struct
{
    tile_type_t type;
    int data;
} tile_t;

typedef struct
{
     int width;
     int height;
     tile_t* tiles;
} tilemap_t;

tile_t ConvertCharToTile(char* tilemap, int width, int height, int x, int y)
{
     tile_t result;
     char c = tilemap[y * width + x];

     switch(c)
     {
     default:
          break;
     case '=':
          result.type = SICK_WALL_BRO;
          result.data = 27;
          break;
     case '-':
          result.type = SICK_WALL_BRO;
          result.data = 31;
          break;
     case '|':
          result.type = SICK_WALL_BRO;
          result.data = 8;
          break;
     case '+':
          result.type = SICK_WALL_BRO;
          result.data = 2;
          break;
     case '.':
          result.type = DOUGHT;
          result.data = 1;
          break;
     case ' ':
          result.type = EMPTY;
          break;
     }

     return result;
}

SDL_Rect GetFrame (int x, int y)
{
    SDL_Rect rect = { x * SPRITE_SIZE, y * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE };
    return rect;
}

SDL_Rect GetWallFrame (int x, int y)
{
    SDL_Rect rect = { x * WALL_SPRITE_SIZE, y * WALL_SPRITE_SIZE, WALL_SPRITE_SIZE, WALL_SPRITE_SIZE };
    return rect;
}

void PrintNumber (int num, int x, int y, SDL_Renderer * renderer, SDL_Texture * texture)
{
    SDL_Rect src = { num * ALPHNUM_SIZE, 0, ALPHNUM_SIZE, ALPHNUM_SIZE };
    SDL_Rect dst = { x, y, ALPHNUM_SIZE, ALPHNUM_SIZE };
    SDL_RenderCopy(renderer, texture, &src, &dst);
}

void DrawPlayer (SDL_Renderer * renderer, SDL_Texture * texture, player_t * player)
{
    SDL_Rect src = GetFrame(player->dir, 3);
    SDL_Rect dst = { player->x, player->y, SPRITE_SIZE, SPRITE_SIZE };
    SDL_RenderCopy(renderer, texture, &src, &dst);
}

void DrawTile(SDL_Renderer * renderer, SDL_Texture * texture, tile_t* tile, int x, int y)
{
    SDL_Rect src = {0, 0, 0, 0};
    SDL_Rect dst = { x, y, WALL_SPRITE_SIZE, WALL_SPRITE_SIZE };

    switch(tile->type)
    {
    default:
        break;
    case EMPTY:
        break;
    case DOUGHT:
        src = GetWallFrame(17, 0);
        break;
    case FRUIT_SNAX:
        break;
    case SICK_WALL_BRO:
        src = GetWallFrame(17, 3);
        break;
    }

    SDL_RenderCopy(renderer, texture, &src, &dst);
}

void DrawTilemap(SDL_Renderer * renderer, SDL_Texture * texture, tilemap_t* tilemap)
{
     int offset_x = 0;
     int offset_y = 0;

     for(int y = 0; y < tilemap->height; y++)
     {
          offset_x = 0;

          for(int x = 0; x < tilemap->width; x++)
          {
               DrawTile(renderer, texture, tilemap->tiles + (y * tilemap->width + x), offset_x, offset_y);

               offset_x += WALL_SPRITE_SIZE;
          }

          offset_y += WALL_SPRITE_SIZE;
     }
}

int main (int argc, char** argv)
{
    (void)(argc);
    (void)(argv);

    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Surface * temp;
    SDL_Texture * sprite_sheet;
    SDL_Event event;
    bool quit;
    player_t player;
    tilemap_t tilemap;

    // our temp test tiles
    char map[] = {
         '+','=','=','=','=','=','=','+',
         '|','.','.','.','.','.','.','|',
         '|','.',' ',' ',' ',' ','.','|',
         '|','.',' ',' ',' ',' ','.','|',
         '|','-','-','-','-',' ','.','|',
         '|','.',' ',' ',' ',' ','.','|',
         '|','.','.','.','.','.','.','|',
         '+','-','-','-','-','-','-','+',
    };
    int map_width = 8;
    int map_height = 8;

    // allocate and convert tiles
    tilemap.tiles = malloc(sizeof(*tilemap.tiles) * map_height * map_width);
    tilemap.width = map_width;
    tilemap.height = map_height;

    for(int i = 0; i < map_height; i++){
         for(int j = 0; j < map_width; j++){
               tilemap.tiles[i * map_width + j] = ConvertCharToTile(map, map_width, map_height, j, i);
         }
    }

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

    int rc = SDL_SetColorKey(temp, SDL_TRUE, SDL_MapRGB(temp->format, 255, 0, 255));
    if (rc != 0) {
        puts("SDL_SetColorKey() failed");
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
    while (!quit) {
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
        DrawTilemap(renderer, sprite_sheet, &tilemap);
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

