#include <iostream>
#include <cstdlib> // For system()
#include <tuple>

#include <SDL.h>
#include "renderer.h"
#include "settings.h"
#include "level.h"

Uint32 floorColour = 0xFF404040;
Uint32 offBlue = 0xFF6464C8;
Uint32 wallColour = 0xFF0000FF;
Uint32 ceilingColour = 0xFF808080;


// function to set the colour of the screen, no bounds checking, uses the global backbuffer
// frequently used so needs to be performant
static void setPixel(int x, int y, Uint32 colour) {
    Uint32* pixels = (Uint32*)backBuffer->pixels;
    pixels[(y * backBuffer->w) + x] = colour;
}
static void drawColumn(int x, int wallHeight, int wallType) {
    if (wallHeight > Settings::getScreenHeight()) {
        wallHeight = Settings::getScreenHeight();
        std::cout << "wall too tall\n";
    }
    if (wallHeight < 0) wallHeight = 0;

    //temp
    Uint32 floorColour = 0xFF606060;
    Uint32 blue = 0xFF0000FF;
    Uint32 offBlue = 0xFF6464C8;
    Uint32 ceilingColour = 0xFF404040;
    Uint32 wallColour;
    if (wallType == 0) {
        wallColour = blue;
    } else if (wallType == 1) {
        wallColour = offBlue;
    }
    //
    int screenHeight = Settings::getScreenHeight();
    int floorTop = (screenHeight-wallHeight) /2;
    int wallTop = floorTop + wallHeight;

    for (int i=0; i < floorTop; i++) {
        setPixel(x, i, floorColour);
    }
    for (int i=floorTop; i < wallTop; i++) {
        setPixel(x, i, wallColour);
    }
    for (int i=wallTop; i < screenHeight; i++) {
        setPixel(x, i, ceilingColour);
    }
}

//temp
int x = 2;
int y = 2;
double facingAngle = 3.14159; //in radians
//
void drawFrame() {
    //temp
    facingAngle = facingAngle + 0.01;
    //

    double degreesPerPixel = Settings::getFov() / Settings::getScreenWidth();
    double raycastAngle = facingAngle - (Settings::getFov()/2); // starting value for leftmost column
    int wallHeight;
    for (int i=0; i < Settings::getScreenWidth(); i++) {
        //std::cout << "raycast distance: " << raycast(x, y, raycastAngle) << "\n" << std::flush;
        //std::cout << "wall height: " << (int)(Settings::getScreenHeight() / raycast(x, y, raycastAngle)) << "\n" << std::flush;

        auto [distance, side] = raycast(x, y, raycastAngle);
        wallHeight = (int)(Settings::getScreenHeight() / distance);
 
        raycastAngle = raycastAngle + degreesPerPixel;



        drawColumn(i, wallHeight, side);
    }
    
    //setPixel(20,20,0xFFFF0000);
}

