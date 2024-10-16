#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>

// Declare the backBuffer globally so other files can use it
extern SDL_Surface* backBuffer;

void drawFrame();

#endif
