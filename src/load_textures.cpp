#include <SDL.h>
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
}
Texture::~Texture() {
    delete[] pixelData;
}

Uint32* Texture::getColumn(double xPosWithinTexture) const {
    int x = (int)(xPosWithinTexture*width);// convert 0-1 to int in textures range

    return &pixelData[x * height];// points to the first pixel in the column
}
int Texture::getHeight() const {
    return height;
}
int Texture::getWidth() const {
    return width;
}
// stop these being global variables
static const std::string wallTextureLocations[] = {"metalgrate1.png", "brickwall1.png", "rusty1.png", "rgb_test.png", "alienWallA.png",
                                                                    "alienControlPanel.png"};
static const std::string spriteTextureLocations[] = {"test_character.png", "test_character.png"};

static const int numberOfWallTextures = std::size(wallTextureLocations);
Texture* wallTextures[numberOfWallTextures];

static const int numberOfSpriteTextures = std::size(spriteTextureLocations);
Texture* spriteTextures[numberOfSpriteTextures];

void loadTextures() {
    
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
    
}
void removeTextures() {
    std::println("hello from remove textures");
    for (int i=0; i < numberOfWallTextures; i++) {
        delete wallTextures[i];
    }
    for (int i=0; i < numberOfSpriteTextures; i++) {
        delete spriteTextures[i];
    }
}