/*
  This file contains the implementation in C of the famous game Snake

  Dipendenze
  - SDL2
  - SDL2_font
*/

#include <stdio.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 900

#define BOARD_WIDTH 20
#define BOARD_HEIGHT 20

#define CELL_WIDTH ((SCREEN_WIDTH / BOARD_WIDTH))
#define CELL_HEIGHT ((SCREEN_HEIGHT / BOARD_HEIGHT))

#define DELAY_FOOD_SPAWN 3
#define FOODS_COUNT 1

// ------------------
// DATA STRUCTURES

typedef struct {
  int x;
  int y;
} Pos;

typedef struct {
  Pos pos;
  int score;
} Food;

typedef struct {
  Pos pos;
  int score;
} Food;

typedef struct {
  Pos pos;
  int score;
} Food;
  
typedef struct {
  Food food[FOODS_COUNT];
  int quit;
} Game;

// ------------------
// FUNCTIONS DECLARATION

void scc(int code);
void *scp(void *ptr);

void init_game(Game *game);
void render_game(SDL_Renderer *renderer, Game *game);
void render_board(SDL_Renderer *renderer);

Pos random_empty_board_pos(Game *game);
Pos random_board_pos();
int pos_is_not_empty(Game* game, Pos p);
int random_int_range(int low, int high);


// ------------------
// GLOBAL VARIABLES

Game GAME = {0};

// ------------------
// UTILS

void scc(int code) {
  if (code < 0) {
    printf("SDL error: %s\n", SDL_GetError());
    exit(1);
  }

  return;
}

void *scp(void *ptr) {
  if (ptr == NULL) {
    printf("SDL error: %s\n", SDL_GetError());
    exit(1);
  }

  return ptr;
}

int random_int_range(int low, int high) {
  return rand() % (end - start) + low;
}

int pos_is_not_empty(Game* game, Pos p) {
  // Food check

  for (int i = 0; i < FOODS_COUNT; i++) {
    if (p.x == game->food[i].pos.x && p.y == game->food[i].pos.y)
      return 1;
  }

  // TODO: Obstacle check

  // TODO: Snake check

  return 0;
}

Pos random_board_pos() {
  Pos p = {0};
  p.x = random_int_range(0, BOARD_WIDTH);
  p.y = random_int_range(0, BOARD_HEIGHT);

  return p;
}

Pos random_empty_board_pos(Game *game) {
  Pos pos = {0};

  do {
    p = random_board_pos();
  } while (pos_is_not_empty(game, p));

  return p;
}

// ------------------
// GAME LOGIC FUNCTIONS

void init_game(Game *game) {
  game->quit = 0;

  init_food(game);
}

void init_foot(Game *game) {
  for (int i = 0; i < FOODS_COUNT; i++) {
    game->food[i].score = 1;
    game->food[i].pos = random_empty_board_pos(game);
  }

  return;
}

// ------------------
// RENDER FUNCTIONS

void render_game(SDL_Renderer *renderer, Game *game) {
  render_board(renderer);
}

void render_board(SDL_Renderer *renderer) {
    scc(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0));
    for (int x = 0; x < BOARD_WIDTH; x++) {
      SDL_RenderDrawLine(renderer, x * CELL_WIDTH, 0, x * CELL_WIDTH, SCREEN_HEIGHT);
    }

    for (int y = 0; y < BOARD_HEIGHT; y++) {
      SDL_RenderDrawLine(renderer, 0, y * CELL_HEIGHT, SCREEN_WIDTH, y * CELL_HEIGHT);
    }
}

int main(void){
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *const window = scp(SDL_CreateWindow("Description", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE));
  SDL_Renderer *const renderer = scp(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));

  init_game(&GAME);

  while (!GAME.quit) {
    SDL_Event event;

    // event handling
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
	GAME.quit = 1;
      }
    }
    
    // update game/renderer
    scc(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0));
    SDL_RenderClear(renderer);
    render_game(renderer, &GAME);

    SDL_RenderPresent(renderer);
    
  }
  
  return 0;
}
