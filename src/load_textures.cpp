#include <SDL.h>
//#include <SDL_image.h>
#include <print>
#include <string>
#include <iostream>
#include "load_textures.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Texture::Texture(unsigned char* stbiImage, int width, int height) : width(width), height(height) {

    pixelData = new Uint32[width * height];

    for (int x=0; x<width; x++) {// this rotates the texture
        for (int y=0; y<height; y++) {
            int i = y * width + x;
            Uint8 r = stbiImage[i * 4 + 0];
            Uint8 g = stbiImage[i * 4 + 1];
            Uint8 b = stbiImage[i * 4 + 2];
            Uint8 a = stbiImage[i * 4 + 3];

            pixelData[x * height + y] = (a << 24) | (r << 16) | (g << 8) | b;// pack into Uint32 (ARGB format)
        }
    }
    //for (int x=0; x<width; x++) {
    //    for (int y=0; y<height; y++) {
    //        Uint32* pixel = (Uint32*)surface->pixels + y * surface->pitch / 4 + x;

            // TEMP HACK TO FIX COLOURS
            //Uint32 colour = *pixel;
            //Uint8 red = (colour >> 0) & 0xFF;
            //Uint8 blue = (colour >> 16) & 0xFF;
            // Swap red and blue components
            //colour = (colour & 0xFF00FF00) | (blue << 0) | (red << 16);
            //*pixel = colour;
            // ENDHACK

    //        pixelData[x * height + y] = *pixel;
    //    }
    //}
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
    //test
    const std::string wallTextureLocations[numberOfWallTextures] = {"metalgrate1.png", "brickwall1.png", "rusty1.png", "rgb_test.png"};
    const std::string spriteTextureLocations[numberOfSpriteTextures] = {"test_character.png", "test_character.png"};
    int width, height, channels;

    for (int i=0; i<numberOfWallTextures+numberOfSpriteTextures; i++) {
        std::string filename;
        if (i<numberOfWallTextures) {
            filename = "textures/" + wallTextureLocations[i];
        } else {
            filename = "textures/" + spriteTextureLocations[i-numberOfWallTextures];
        }
        
        
        unsigned char* stbiImage = stbi_load(filename.c_str(), &width, &height, &channels, 4);

        if (!stbiImage) {
            std::cerr << "Failed to load image: " << stbi_failure_reason() << std::endl;
            return;
        }
        // Print out some details
        std::println("Loaded image: {}", filename);
        std::println("Dimensions: {}x{}", width, height);
        std::println("Channels: {}", channels);

        if (i<numberOfWallTextures) {
            wallTextures[i] = new Texture(stbiImage, width, height);
        } else {
            spriteTextures[i-numberOfWallTextures] = new Texture(stbiImage, width, height);
        }

        stbi_image_free(stbiImage);
    }
    //End test
    //const std::string wall_texture_locations[numberOfWallTextures] = {"metalgrate1.png", "brickwall1.png", "rusty1.png", "rgb_test.png"};

    //for (int i=0; i<numberOfWallTextures; i++) {
    //    std::string filename = "textures/" + wall_texture_locations[i];
    //    SDL_Surface* surface = IMG_Load(filename.c_str());
    //    if (!surface) {
    //        throw std::ios_base::failure("Image load error: " + filename);
    //    }
    //    // Log if the pixel format is wrong
    //    //SDL_PixelFormatEnum format = surface->format->format;
    //    SDL_Surface* in_correct_format = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);

    //    if (!in_correct_format) {
    //        std::cerr << "Error: Failed to convert surface format!" << std::endl;
    //        return;  // Return or handle the error as necessary
    //    }

        
    //    wallTextures[i] = new Texture(in_correct_format);
    //    SDL_FreeSurface(in_correct_format);
    //    SDL_FreeSurface(surface);

    //}
    
    //const std::string spriteTextureLocations[numberOfSpriteTextures] = {"test_character.png", "test_character.png"};

    //for (int i=0; i<numberOfSpriteTextures; i++) {
    //    std::string filename = "textures/" + spriteTextureLocations[i];
    //    SDL_Surface* surface = IMG_Load(filename.c_str()); // Stop using IMG_Load use custom code, only need to support png and can drop sdl image dependency
    //    if (!surface) {
    //        throw std::ios_base::failure("Image load error: " + filename);
    //    }
    //    // Log if the pixel format is wrong
    //    //SDL_PixelFormatEnum format = surface->format->format;
    //    SDL_Surface* in_correct_format = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);

    //    if (!in_correct_format) {
    //        std::cerr << "Error: Failed to convert surface format!" << std::endl;
    //        return;  // Return or handle the error as necessary
    //    }

        
    //    spriteTextures[i] = new Texture(in_correct_format);
    //    SDL_FreeSurface(in_correct_format);
    //    SDL_FreeSurface(surface);

    //}
    //std::cout << spriteTextures[0] << std::endl;
    //std::cout << spriteTextures[1] << std::endl;
    //spriteTextures[spriteNum]->getColumn(xPosWithinTexture);
}
// note: we are should probably clean up the texture data on program close
//void removeTextures() {}