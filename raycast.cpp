#include "main.h"
#include <cmath>
#include <stdio.h>

void raycast(SDL_Instance *instance) {
    for (int x = 0; x < 640; x++) {
        double cameraX = 2 * x / (double)640 - 1;
        double rayDirX = player.dirX + player.planeX * cameraX;
        double rayDirY = player.dirY + player.planeY * cameraX;

        int mapX = (int)player.posX;
        int mapY = (int)player.posY;

        double sideDistX, sideDistY;

        double deltaDistX = fabs(1 / rayDirX);
        double deltaDistY = fabs(1 / rayDirY);
        double perpWallDist;

        int stepX, stepY;

        int hit = 0;
        int side;

        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (player.posX - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - player.posX) * deltaDistX;
        }
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (player.posY - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - player.posY) * deltaDistY;
        }

        while (hit == 0) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            if (worldMap[mapX][mapY] > 0) hit = 1;
        }

        if (side == 0) perpWallDist = (mapX - player.posX + (1 - stepX) / 2) / rayDirX;
        else perpWallDist = (mapY - player.posY + (1 - stepY) / 2) / rayDirY;

        int lineHeight = (int)(480 / perpWallDist);

        int drawStart = -lineHeight / 2 + 480 / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + 480 / 2;
        if (drawEnd >= 480) drawEnd = 479;

        int wallType = worldMap[mapX][mapY];
        if (wallType == 1) {
            SDL_SetRenderDrawColor(instance->renderer, 255, 0, 0, 255); // Red for wall type 1
        } else if (wallType == 2) {
            SDL_SetRenderDrawColor(instance->renderer, 0, 0, 255, 255); // Blue for wall type 2
        } else {
            SDL_SetRenderDrawColor(instance->renderer, 255, 255, 255, 255); // Default color for other wall types
        }
        SDL_RenderDrawLine(instance->renderer, x, drawStart, x, drawEnd);

    }
}
