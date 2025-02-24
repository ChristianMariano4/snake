/*
  This file contains the implementation in C of the famous game Snake

  Dependencies:
  - SDL2
  - SDL2_font
*/

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 900

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 10

#define CELL_WIDTH ((SCREEN_WIDTH / BOARD_WIDTH))
#define CELL_HEIGHT ((SCREEN_HEIGHT / BOARD_HEIGHT))

#define DELAY_FOOD_SPAWN 3
#define FOODS_COUNT 1

// ------------------
// DATA STRUCTURES

typedef struct
{
  int x;
  int y;
} Pos;

typedef struct
{
  Pos pos;
  int score;
} Food;

typedef struct
{
  Food food[FOODS_COUNT];
  int quit;
} Game;

// ------------------
// FUNCTIONS DECLARATION

void scc(int code);
void *scp(void *ptr);

int random_int_range(int low, int high);
Pos random_board_pos();
int pos_is_not_empty(Game *game, Pos p);
Pos random_empty_board_pos(Game *game);

void init_game(Game *game);
void init_food(Game *game);

void render_game(SDL_Renderer *renderer, Game *game);
void render_food(SDL_Renderer *renderer, Game *game);
void remove_food(SDL_Renderer *renderer, Game *game);
void render_board(SDL_Renderer *renderer);

// ------------------
// GLOBAL VARIABLES

Game GAME = {0};

// ------------------
// UTILS

void scc(int code)
{
  if (code < 0)
  {
    printf("SDL error: %s\n", SDL_GetError());
    exit(1);
  }

  return;
}

void *scp(void *ptr)
{
  if (ptr == NULL)
  {
    printf("SDL error: %s\n", SDL_GetError());
    exit(1);
  }

  return ptr;
}

int random_int_range(int low, int high)
{
  return (rand() % (high - low)) + low;
}

int pos_is_not_empty(Game *game, Pos p)
{
  // Food check

  for (int i = 0; i < FOODS_COUNT; i++)
  {
    if (p.x == game->food[i].pos.x && p.y == game->food[i].pos.y)
      return 1;
  }

  // TODO: Obstacle check

  // TODO: Snake check

  return 0;
}

Pos random_board_pos()
{
  Pos p = {0};
  p.x = random_int_range(0, BOARD_WIDTH);
  p.y = random_int_range(0, BOARD_HEIGHT);

  return p;
}

Pos random_empty_board_pos(Game *game)
{
  Pos p = {0};

  do
  {
    p = random_board_pos();
  } while (pos_is_not_empty(game, p));

  return p;
}

// ------------------
// GAME LOGIC FUNCTIONS

void init_game(Game *game)
{
  game->quit = 0;

  init_food(game);
}

void init_food(Game *game)
{
  for (int i = 0; i < FOODS_COUNT; i++)
  {
    game->food[i].score = 1;
    game->food[i].pos = random_empty_board_pos(game);
  }

  return;
}

int allow_refresh_food(void)
{
  static struct timeval old_t = {0}; // static is needed to have persistent memory across different calls
  static struct timeval new_t = {0};
  static int init = -1;
  Uint32 time_elapsed = -1;

  if (init == -1)
  {
    init = 1;
    gettimeofday(&old_t, NULL);
    return 1;
  }

  gettimeofday(&new_t, NULL);
  time_elapsed = (double)(new_t.tv_usec - old_t.tv_usec) / 1000000 + (double)(new_t.tv_sec - old_t.tv_sec); // secs between two calls

  if (time_elapsed < DELAY_FOOD_SPAWN)
  {
    return 0;
  }
  else
  {
    old_t = new_t; // update with current timestamp
    return 1;
  }
}

void update_food(SDL_Renderer *renderer, Game *game)
{
  if (allow_refresh_food())
  {
    remove_food(renderer, game);
    init_food(game);
  }
}

// ------------------
// RENDER FUNCTIONS

void render_game(SDL_Renderer *renderer, Game *game)
{
  render_board(renderer);

  render_food(renderer, game);
}

void render_board(SDL_Renderer *renderer)
{
  scc(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0));
  for (int x = 0; x < BOARD_WIDTH; x++)
  {
    SDL_RenderDrawLine(renderer, x * CELL_WIDTH, 0, x * CELL_WIDTH, SCREEN_HEIGHT);
  }

  for (int y = 0; y < BOARD_HEIGHT; y++)
  {
    SDL_RenderDrawLine(renderer, 0, y * CELL_HEIGHT, SCREEN_WIDTH, y * CELL_HEIGHT);
  }
}

void render_food(SDL_Renderer *renderer, Game *game)
{
  scc(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0));

  for (int i = 0; i < FOODS_COUNT; i++)
  {
    Food f = game->food[i];

    if (f.score == 0)
    {
      continue;
    }

    SDL_Rect rect = {
        (int)floorf(f.pos.x * CELL_WIDTH),
        (int)floorf(f.pos.y * CELL_HEIGHT),
        (int)floorf(CELL_WIDTH),
        (int)floorf(CELL_HEIGHT)};

    scc(SDL_RenderFillRect(renderer, &rect));
  }
}

void remove_food(SDL_Renderer *renderer, Game *game)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  for (int i = 0; i < FOODS_COUNT; i++)
  {
    Food f = game->food[i];

    if (!f.score)
      continue;

    SDL_Rect rect = {
        (int)floorf(f.pos.x * CELL_WIDTH),
        (int)floorf(f.pos.y * CELL_HEIGHT),
        (int)floorf(CELL_WIDTH),
        (int)floorf(CELL_HEIGHT)};

    scc(SDL_RenderFillRect(renderer, &rect));
  }
}

int main(void)
{
  srand(time(0));
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *const window = scp(SDL_CreateWindow("Description...", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE));
  SDL_Renderer *const renderer = scp(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));

  init_game(&GAME);

  while (!GAME.quit)
  {
    SDL_Event event;

    // event handling
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        GAME.quit = 1;
      }
    }

    // update game/renderer
    scc(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0));
    SDL_RenderClear(renderer);

    update_food(renderer, &GAME);

    render_game(renderer, &GAME);

    SDL_RenderPresent(renderer);
  }

  return 0;
}
