#include <iostream>
//#include <cstdlib> // For system()
#include <tuple>

#include <SDL.h>
#include "renderer.h"
#include "settings.h"
#include "level.h"

static Uint32 floorColour = 0xFF606060;
static Uint32 blue = 0xFF6464C8;
static Uint32 offBlue = 0xFF0000FF;
static Uint32 ceilingColour = 0xFF404040;


// function to set the colour of the screen, no bounds checking, uses the global backbuffer
// frequently used so needs to be performant
static void setPixel(int x, int y, Uint32 colour) {
    Uint32* pixels = (Uint32*)backBuffer->pixels;
    pixels[(y * backBuffer->w) + x] = colour;
}
static void drawColumn(int x, int wallHeight, int wallType) {
    if (wallHeight > Settings::getScreenHeight()) {
        wallHeight = Settings::getScreenHeight();
    }
    if (wallHeight < 0) wallHeight = 0;

    Uint32 wallColour;
    if (wallType == 0) {
        wallColour = blue;
    } else { //if (wallType == 1) {
        wallColour = offBlue;
    }

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


void drawFrame(double positionX, double positionY, double rotation) {
    

    double degreesPerPixel = Settings::getFov() / Settings::getScreenWidth();
    double raycastAngle = rotation - (Settings::getFov()/2); // starting value for leftmost column
    int wallHeight;
    for (int i=0; i < Settings::getScreenWidth(); i++) {
        //std::cout << "raycast distance: " << raycast(x, y, raycastAngle) << "\n" << std::flush;
        //std::cout << "wall height: " << (int)(Settings::getScreenHeight() / raycast(x, y, raycastAngle)) << "\n" << std::flush;

        auto [distance, side] = raycast(positionX, positionY, raycastAngle);
        distance = distance * cos((raycastAngle - rotation) * 0.95); // fish eye effect corrections (0.95 keeps the effect a tiny bit)

        wallHeight = (int)(1.5 * Settings::getScreenHeight() / distance);
 
        raycastAngle = raycastAngle + degreesPerPixel;

        drawColumn(i, wallHeight, side);
    }
    
    setPixel(Settings::getScreenWidth()/2, Settings::getScreenHeight()/2,0xFFFF0000);
}

