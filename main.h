#ifndef MAIN_H
#define MAIN_H

#include <SDL.h>
#include <vector>
typedef struct {
    double posX, posY;  // player position
    double dirX, dirY;  // player direction vector
    double planeX, planeY;  // camera plane
} Player;

typedef struct SDL_Instance {
    SDL_Window *window;
    SDL_Renderer *renderer;
} SDL_Instance;

extern int worldMap[24][24];
extern Player player;

bool initialize_SDL(SDL_Instance *instance);
void cleanup(SDL_Instance *instance);
void raycast(SDL_Instance *instance);
void handle_input(bool *quit);

#endif // MAIN_H
