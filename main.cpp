#include <SDL.h>
#include <cstdio>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "main.h"

// Function to generate a random map
std::vector<std::vector<int> > generateMap(int width, int height) {
    std::vector<std::vector<int> > map(height, std::vector<int>(width, 0));

    // Set the borders to be walls
    for (int x = 0; x < width; x++) {
        map[0][x] = 1;
        map[height - 1][x] = 1;
    }
    for (int y = 0; y < height; y++) {
        map[y][0] = 1;
        map[y][width - 1] = 1;
    }

    // Fill the inside with random walls and empty spaces
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            map[y][x] = rand() % 2; // 0 for empty, 1 for wall
        }
    }

    return map;
}

// Function to convert the map to a format suitable for the raycasting code
void convertMap(const std::vector<std::vector<int> >& generatedMap, int worldMap[24][24]) {
    for (int y = 0; y < 24; y++) {
        for (int x = 0; x < 24; x++) {
            worldMap[y][x] = generatedMap[y][x];
        }
    }
}

int worldMap[24][24];

// Player initialization
Player player = {1.5, 1.5, -1, 0, 0, 0.6};

// Initialize SDL
bool initialize_SDL(SDL_Instance *instance) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return false;
    }

    instance->window = SDL_CreateWindow("Maze MVP", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    if (!instance->window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    instance->renderer = SDL_CreateRenderer(instance->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!instance->renderer) {
        SDL_DestroyWindow(instance->window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }
    return true;
}

// Cleanup SDL
void cleanup(SDL_Instance *instance) {
    SDL_DestroyRenderer(instance->renderer);
    SDL_DestroyWindow(instance->window);
    SDL_Quit();
}

// Draw scene
void draw_scene(SDL_Instance *instance) {
    SDL_SetRenderDrawColor(instance->renderer, 0, 0, 0, 255);
    SDL_RenderClear(instance->renderer);

    raycast(instance);

    SDL_RenderPresent(instance->renderer);
}

// Handle input
void handle_input(bool *quit) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *quit = true;
        }
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            const Uint8 *state = SDL_GetKeyboardState(NULL);

            double moveSpeed = 0.05; // Adjust as necessary
            double rotSpeed = 0.03;  // Adjust as necessary

            if (state[SDL_SCANCODE_ESCAPE]) {
                *quit = true;
            }
            if (state[SDL_SCANCODE_W]) {
                if (worldMap[int(player.posX + player.dirX * moveSpeed)][int(player.posY)] == 0) player.posX += player.dirX * moveSpeed;
                if (worldMap[int(player.posX)][int(player.posY + player.dirY * moveSpeed)] == 0) player.posY += player.dirY * moveSpeed;
            }
            if (state[SDL_SCANCODE_S]) {
                if (worldMap[int(player.posX - player.dirX * moveSpeed)][int(player.posY)] == 0) player.posX -= player.dirX * moveSpeed;
                if (worldMap[int(player.posX)][int(player.posY - player.dirY * moveSpeed)] == 0) player.posY -= player.dirY * moveSpeed;
            }
            if (state[SDL_SCANCODE_A]) {
                double oldDirX = player.dirX;
                player.dirX = player.dirX * cos(rotSpeed) - player.dirY * sin(rotSpeed);
                player.dirY = oldDirX * sin(rotSpeed) + player.dirY * cos(rotSpeed);
                double oldPlaneX = player.planeX;
                player.planeX = player.planeX * cos(rotSpeed) - player.planeY * sin(rotSpeed);
                player.planeY = oldPlaneX * sin(rotSpeed) + player.planeY * cos(rotSpeed);
            }
            if (state[SDL_SCANCODE_D]) {
                double oldDirX = player.dirX;
                player.dirX = player.dirX * cos(-rotSpeed) - player.dirY * sin(-rotSpeed);
                player.dirY = oldDirX * sin(-rotSpeed) + player.dirY * cos(-rotSpeed);
                double oldPlaneX = player.planeX;
                player.planeX = player.planeX * cos(-rotSpeed) - player.planeY * sin(-rotSpeed);
                player.planeY = oldPlaneX * sin(-rotSpeed) + player.planeY * cos(-rotSpeed);
            }

        }
    }
}

// Main function
int main(int argc, char* argv[]) {
    srand(time(0)); // Seed the random number generator

    // Generate a new map
    std::vector<std::vector<int> > generatedMap = generateMap(24, 24);

    // Convert the generated map to the format needed for your raycasting code
    convertMap(generatedMap, worldMap);
    //print map
    for (int i = 0 ; i < 24 ; i++){
            for (int j = 0 ; j < 24 ; j++){
                if(worldMap[i][j] == 0)
                    printf(" ");
                else
                   printf("#");
            }
            printf("\n");
    }
    SDL_Instance instance;
    if (!initialize_SDL(&instance)) {
        return 1;
    }

    bool quit = false;

    while (!quit) {
        handle_input(&quit);
        draw_scene(&instance);
        SDL_Delay(16);  // Cap frame rate to ~60 fps
    }

    cleanup(&instance);
    return 0;
}
