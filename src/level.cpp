#include <cmath>
#include <tuple>

#include "level.h"
#include "settings.h"

//static int level[15][15] = {
//    {0,0,0,0,0,1,0,0,0,1,0,0,0,0,0},
//    {0,1,1,0,0,0,1,0,1,0,0,0,1,1,0},
//    {0,1,1,0,0,0,0,1,0,0,0,0,1,1,0},
//    {0,1,0,0,0,0,0,0,0,0,0,0,0,1,0},
//    {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
//    {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
//    {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
//    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
//    {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
//    {0,0,0,1,0,0,0,0,0,1,0,0,0,1,0},
//    {0,0,0,0,1,0,0,1,0,1,0,0,0,1,0},
//    {0,0,0,0,1,0,0,1,0,1,0,0,0,1,0},
//    {0,0,0,0,1,1,1,1,1,1,0,0,0,1,0},
//    {0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
//    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//};
static int level[15][15] = {
    {0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
    {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
    {1,1,1,1,0,0,0,0,0,0,0,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
    {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1}
};
std::tuple<double, int> raycast(double rayX, double rayY, double angle) {
    //The out of bounds areas of the level array
    int levelSizeX = sizeof(level[0])/sizeof(level[0][0]);
    int levelSizeY = sizeof(level) / sizeof(level[0]);
    
    //direction of the ray (unit vector)
    double rayDirX = cos(angle);
    double rayDirY = sin(angle);

    //which box of the map we're in
    int mapX = int(rayX);
    int mapY = int(rayY);

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
        sideDistX = (rayX - mapX) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (mapX + 1.0 - rayX) * deltaDistX;
    }
    if(rayDirY < 0) {
        stepY = -1;
        sideDistY = (rayY - mapY) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (mapY + 1.0 - rayY) * deltaDistY;
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
        if (mapX < 0 || mapX >= levelSizeX || mapY < 0 || mapY >= levelSizeY) {
            hit = 1;
            break;
        }
    }
    
    //Calculate distance of perpendicular ray (Euclidean distance would give fisheye effect)
    double perpWallDist;
    if(side == 0) perpWallDist = (sideDistX - deltaDistX);
    else          perpWallDist = (sideDistY - deltaDistY);



    return std::make_tuple(perpWallDist, side);

}
