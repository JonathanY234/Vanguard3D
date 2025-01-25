#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include "load_textures.h"


Texture::Texture(SDL_Surface* surface) {
    width = surface->w;
    height = surface->h;

    pixelData = new Uint32[width * height];

    for (int x=0; x<width; x++) {
        for (int y=0; y<height; y++) {
            Uint32* pixel = (Uint32*)surface->pixels + y * surface->pitch / 4 + x; // need to fix rotated textures 

            // TEMP HACK TO FIX COLOURS
            Uint32 colour = *pixel;
            Uint8 red = (colour >> 0) & 0xFF;
            Uint8 blue = (colour >> 16) & 0xFF;
            // Swap red and blue components
            colour = (colour & 0xFF00FF00) | (blue << 0) | (red << 16);
            *pixel = colour;
            // ENDHACK

            pixelData[x * height + y] = *pixel;
        }
    }
}
Uint32* Texture::getColumn(double xPosWithinTexture) const {
    int x = (int)(xPosWithinTexture*width);// convert 0-1 to int in textures range

    return &pixelData[x * height];// points to the first pixel in the column
}
Uint32 Texture::test_getPixel(int x, int y) {
    return 0;
    //return pixelData[y][x];
}
int Texture::getHeight() const {
    return height;
}
int Texture::getWidth() const {
    return width;
}
static const int numberOfWallTextures = 4;
Texture* wallTextures[numberOfWallTextures];

static const int numberOfSpriteTextures = 2;
Texture* spriteTextures[numberOfSpriteTextures];

void loadTextures() {
    
    const std::string wall_texture_locations[numberOfWallTextures] = {"metalgrate1.png", "brickwall1.png", "rusty1.png", "rgb_test.png"};

    for (int i=0; i<numberOfWallTextures; i++) {
        std::string filename = "textures/" + wall_texture_locations[i];
        SDL_Surface* surface = IMG_Load(filename.c_str()); // Stop using IMG_Load, only need to support png and can drop sdl image dependency
        if (!surface) {
            throw std::ios_base::failure("Image load error: " + filename);
        }
        // Log if the pixel format is wrong
        //SDL_PixelFormatEnum format = surface->format->format;
        SDL_Surface* in_correct_format = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);

        if (!in_correct_format) {
            std::cerr << "Error: Failed to convert surface format!" << std::endl;
            return;  // Return or handle the error as necessary
        }

        
        wallTextures[i] = new Texture(in_correct_format);
        SDL_FreeSurface(in_correct_format);
        SDL_FreeSurface(surface);

    }
    
    const std::string spriteTextureLocations[numberOfSpriteTextures] = {"test_character.png", "test_character.png"};

    for (int i=0; i<numberOfSpriteTextures; i++) {
        std::string filename = "textures/" + spriteTextureLocations[i];
        SDL_Surface* surface = IMG_Load(filename.c_str()); // Stop using IMG_Load use custom code, only need to support png and can drop sdl image dependency
        if (!surface) {
            throw std::ios_base::failure("Image load error: " + filename);
        }
        // Log if the pixel format is wrong
        //SDL_PixelFormatEnum format = surface->format->format;
        SDL_Surface* in_correct_format = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);

        if (!in_correct_format) {
            std::cerr << "Error: Failed to convert surface format!" << std::endl;
            return;  // Return or handle the error as necessary
        }

        
        spriteTextures[i] = new Texture(in_correct_format);
        SDL_FreeSurface(in_correct_format);
        SDL_FreeSurface(surface);

    }
    //std::cout << spriteTextures[0] << std::endl;
    //std::cout << spriteTextures[1] << std::endl;
    //spriteTextures[spriteNum]->getColumn(xPosWithinTexture);
}
// note: we are should probably clean up the texture data on program close
//void removeTextures() {}