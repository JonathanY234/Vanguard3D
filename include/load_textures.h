#ifndef LOAD_TEXTURES_H
#define LOAD_TEXTURES_H

#include <SDL.h>

class Texture {
private:
    Uint32* pixelData;// pointer to array
    int width, height;
public:
    Texture(unsigned char* stbiImage, int width, int height);
    ~Texture();
    Uint32* getColumn(double xPosWithinTexture) const;
    int getWidth() const;
    int getHeight() const;
};
extern Texture* wallTextures[];
extern Texture* spriteTextures[];
void loadTextures();
void removeTextures();

#endif // LOAD_TEXTURES_H