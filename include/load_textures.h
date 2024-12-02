#ifndef LOAD_TEXTURES_H
#define LOAD_TEXTURES_H

#include <SDL.h>
#include <vector>

class Texture {
private:
    std::vector<std::vector<Uint32>> pixelData;
    int width, height;
public:
    Texture(SDL_Surface* surface);
    std::vector<Uint32> getColumn(double xPosWithinTexture) const;
    Uint32 test_getPixel(int x, int y);
};
extern Texture* wallTextures[];
extern Texture* spriteTextures[];
void loadTextures();

#endif // LOAD_TEXTURES_H