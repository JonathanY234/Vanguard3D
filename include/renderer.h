// renderer.h
#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>



void initializeRenderer(SDL_Surface* backBuffer);
void renderFrame(SDL_Surface* backBuffer);

#endif // RENDERER_H
