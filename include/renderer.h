#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include <memory>
#include "game.h" //tell renderer about Player class

// Declare the backBuffer globally so other files can use it
extern SDL_Surface* backBuffer;

void initialiseSprites();
void drawFrame(double positionX, double positionY, double rotation);

#endif
