#include <cmath>
#include <tuple>
#include <vector>
#include "level.h"
#include "settings.h"
#include "game.h"

//temp
#include <iostream>

//static int level[2][30] = {
//    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
//};
static constexpr int level[17][17] = {
    {1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2},
    {1,0,0,0,0,1,0,0,0,0,0,2,0,0,0,0,2},
    {1,0,0,0,0,1,0,0,0,0,0,2,0,0,0,0,2},
    {1,0,-1,0,0,0,0,0,0,0,0,2,0,0,0,0,2},
    {1,0,0,0,0,1,0,0,0,0,0,2,0,0,0,0,2},
    {1,0,0,0,0,1,0,0,0,0,0,2,0,0,0,0,2},
    {1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,2},
    {1,0,0,0,0,1,0,0,0,0,0,2,0,0,0,0,2},
    {1,0,0,0,0,1,0,0,0,0,0,2,0,0,0,0,2},
    {1,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,2},
    {1,0,0,0,0,1,0,0,0,0,0,2,0,0,0,0,2},
    {1,0,0,0,0,1,0,0,0,0,0,2,0,0,0,0,2},
    {1,1,1,1,1,0,0,0,0,0,0,0,2,2,2,2,2},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,0,1,1,0,3,1,0,1,1,0,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
}; // need to enforce levels bounded by walls
// Get the out of bounds areas of the level array
static constexpr int levelSizeX = sizeof(level[0])/sizeof(level[0][0]);
static constexpr int levelSizeY = sizeof(level) / sizeof(level[0]);

bool isWall(int x, int y) {
    // when double is passed it is truncated to int

    //if (x < 0 || y < 0 || x >= levelSizeX || y >= levelSizeY) { // not used because we enforce areas surrounded by walls
    //    return true;
    //}
    return level[x][y] > 0;
}
std::tuple<double, int, double, int> raycast(double rayStartX, double rayStartY, double angle) {
    
    //direction of the ray (unit vector)
    double rayDirX = cos(angle);
    double rayDirY = sin(angle);

    //which box of the map we're in
    int mapX = int(rayStartX);
    int mapY = int(rayStartY);

    //length of ray from current position to next x or y-side
    double sideDistX;
    double sideDistY;

    //length of ray from one x or y-side to next x or y-side
    double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
    double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);
    
    //what direction to step in x or y-direction (either +1 or -1)
    int stepX;
    int stepY;

    int hit = 0; //was there a wall hit?
    int side; //was a NS or a EW wall hit?

    //calculate step and initial sideDist
    if(rayDirX < 0) {
        stepX = -1;
        sideDistX = (rayStartX - mapX) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (mapX + 1.0 - rayStartX) * deltaDistX;
    }
    if(rayDirY < 0) {
        stepY = -1;
        sideDistY = (rayStartY - mapY) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (mapY + 1.0 - rayStartY) * deltaDistY;
    }
    //perform DDA
    while (hit == 0) {
        //jump to next map square, either in x-direction, or in y-direction
        if(sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            mapX += stepX;
            side = 0;
        } else {
            sideDistY += deltaDistY;
            mapY += stepY;
            side = 1;
        }
        //Check if ray has hit a wall
        if(level[mapX][mapY] > 0) hit = 1;

        // Check if ray is out of bounds and treat that as a hit 
        // could optimise by requiring levels to be surrounded by walls
        // check at launch that all maps are surrounded by walls, to avoid human error 
        //if (mapX < 0 || mapX >= levelSizeX || mapY < 0 || mapY >= levelSizeY) {
        //    hit = 1;
        //    break;
        //}
    }
    
    // Calculate distance of perpendicular ray
    double perpWallDist;
    if(side == 0) perpWallDist = (sideDistX - deltaDistX);
    else          perpWallDist = (sideDistY - deltaDistY);

    // texture info
    double xPosWithinTexture;
    
    if (side == 1) { // clean up
        double test = rayStartX + perpWallDist * rayDirX;
        xPosWithinTexture = test - std::floor(test);
    } else {
        double test = rayStartY + perpWallDist * rayDirY;

        xPosWithinTexture = test - std::floor(test);
    }
    
    return std::make_tuple(perpWallDist, side, xPosWithinTexture, level[mapX][mapY]);
}

void getSprites(std::vector<Sprite>& sprites) {
    for (int y=0; y<levelSizeY; y++) {
        for (int x=0; x<levelSizeX; x++) {
            if (level[x][y] < 0) {
                sprites.push_back(Sprite(x,y,0, 0.35,level[x][y]));
            }
        }
    }
}