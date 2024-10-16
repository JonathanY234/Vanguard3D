#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>

// Declare the backBuffer globally so other files can use it
extern SDL_Surface* backBuffer;

void setPixel(int x, int y, Uint32 color);
void drawColumn(int x, int height, int wallHeight);
void drawFrame();

#endif
