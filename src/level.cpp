#include <cmath>

#include "level.h"
#include "settings.h"

static int level[10][10] = {
    {0,0,1,0,0,1,1,0,0,0},
    {0,0,1,1,1,1,1,0,0,0},
    {0,1,1,0,0,0,0,0,0,0},
    {0,1,1,0,0,1,1,0,0,0},
    {0,1,0,0,0,0,1,0,0,0},
    {0,1,0,0,0,0,1,0,0,0},
    {0,1,1,0,0,1,1,0,0,0},
    {0,1,1,0,0,1,1,0,0,0},
    {0,1,1,0,0,1,1,0,0,0},
    {0,1,1,1,1,1,1,0,0,0}
};
double raycast(double x, double y, double angle) {
    // Direction of the ray (unit vector)
    double rayDirX = cos(angle);
    double rayDirY = sin(angle);

    // Grid stepping
    int gridX = static_cast<int>(x);
    int gridY = static_cast<int>(y);

    // Level dimensions (adjust this based on your grid size)
    const int levelWidth = 10;
    const int levelHeight = 10;

    // Calculate the distance to the next x and y grid lines
    double deltaDistX = std::abs(1.0 / rayDirX);
    double deltaDistY = std::abs(1.0 / rayDirY);

    // Determine step direction (+1 for forward, -1 for backward)
    int stepX, stepY;
    double sideDistX, sideDistY;

    if (rayDirX < 0) {
        stepX = -1;
        sideDistX = (x - gridX) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (gridX + 1.0 - x) * deltaDistX;
    }

    if (rayDirY < 0) {
        stepY = -1;
        sideDistY = (y - gridY) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (gridY + 1.0 - y) * deltaDistY;
    }

    // DDA loop to step through the grid
    bool hit = false;
    int side;

    // Make sure we're inside the grid
    while (!hit) {
        // Step to the next grid line (either in x or y direction)
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            gridX += stepX;
            side = 0; // Vertical wall hit
        } else {
            sideDistY += deltaDistY;
            gridY += stepY;
            side = 1; // Horizontal wall hit
        }

        // Check boundaries to prevent out-of-bounds access
        if (gridX < 0 || gridX >= levelWidth || gridY < 0 || gridY >= levelHeight) {
            // Ray went out of bounds
            return -1; // Indicating no wall was hit within bounds
        }

        // Check if we hit a wall (assuming 'level' is a global grid)
        if (level[gridX][gridY] == 1) {
            hit = true;
        }
    }

    // Calculate the distance to the wall
    double distance;
    if (side == 0) {
        distance = (gridX - x + (1 - stepX) / 2) / rayDirX;
    } else {
        distance = (gridY - y + (1 - stepY) / 2) / rayDirY;
    }

    return distance;
}
