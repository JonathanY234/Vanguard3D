#include <SDL.h>
#include "renderer.h"

Uint32 floorColour = 0xFF404040;
Uint32 wallColour = 0xFF0000FF;
Uint32 ceilingColour = 0xFF808080;
int screenHeight = 600;

// function to set the colour of the screen, no bounds checking, uses the global backbuffer
// frequently used so needs to be performant
void setPixel(int x, int y, Uint32 colour) {
    Uint32* pixels = (Uint32*)backBuffer->pixels;
    pixels[(y * backBuffer->w) + x] = colour;
}
void drawColumn(int x, int screenHeight, int wallHeight) {
    Uint32 floorColour = 0xFF404040;
    Uint32 wallColour = 0xFF0000FF;
    Uint32 ceilingColour = 0xFF808080;

    int floorTop = (screenHeight-wallHeight) /2;
    int wallTop = floorTop + wallHeight;

    for (int i=0; i < floorTop; i++) {
        setPixel(x, i, floorColour);
    }
    for (int i=floorTop+1; i < wallTop; i++) {
        setPixel(x, i, wallColour);
    }
    for (int i=wallTop+1; i < screenHeight; i++) {
        setPixel(x, i, ceilingColour);
    }
}

void drawFrame() {
    setPixel(20,20,0xFFFF0000);
    drawColumn(30, screenHeight, 300);
    drawColumn(31, screenHeight, 250);
}

