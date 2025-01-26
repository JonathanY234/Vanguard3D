#ifndef LOAD_TEXTURES_H
#define LOAD_TEXTURES_H

#include <SDL.h>
#include <vector>

class Texture {
private:
    Uint32* pixelData;// pointer to array
    int width, height;
public:
    Texture(unsigned char* stbiImage, int width, int height);
    Uint32* getColumn(double xPosWithinTexture) const;
    Uint32 test_getPixel(int x, int y);
    int getWidth() const;
    int getHeight() const;
};
extern Texture* wallTextures[];
extern Texture* spriteTextures[];
void loadTextures();

#endif // LOAD_TEXTURES_H