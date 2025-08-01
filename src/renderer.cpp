#include <print>
#include <tuple>
#include <cassert>
#include <cmath>

#include <SDL.h>
#include "renderer.h"
#include "settings.h"
#include "level.h"
#include "load_textures.h"

static Uint32 floorColour = 0xFF606060;
static Uint32 ceilingColour = 0xFF404040;

static std::vector<Sprite*> sprites;// destructor not implemented
void initialiseSprites() {
    getSprites(sprites);// from level
}


// function to set the colour of the screen, no bounds checking, uses the global backbuffer
static inline void setPixel(int x, int y, Uint32 colour) {
    Uint32* pixels = (Uint32*)backBuffer->pixels;
    pixels[(y * backBuffer->w) + x] = colour;
}

void drawTexturedColumn(int x, int unboundedTop, int unboundedBottom, double xPosWithinTexture, bool wallNotSprite, int TexNum, double pixel_gap) {
    /* draws a whole column of pixels to the screen within bounds from a texture
       no input checking... yet
    */
    Uint32* pixels = (Uint32*)backBuffer->pixels;
    int ScreenWidth = backBuffer->w;

    int top = std::max(unboundedTop, 0);
    int bottom = std::min(unboundedBottom, backBuffer->h);
    
    //get pointer to starting spot in backbuffer
    Uint32* pixelPtr = &pixels[(top * ScreenWidth) + x];

    //get pointer to the texture column
    const Uint32* textureColumn;
    if (wallNotSprite) {
        textureColumn = wallTextures[TexNum]->getColumn(xPosWithinTexture);
    } else {
        textureColumn = spriteTextures[TexNum]->getColumn(xPosWithinTexture);
    }
    
    for (int i=top; i < bottom; i++) {
        int textureY = std::round((i-unboundedTop)*pixel_gap); //nearest neighbour scaling

        *pixelPtr = textureColumn[textureY]; // draw the pixel
        pixelPtr += ScreenWidth;
    }
}
void drawRoofOrFloor() {
    // todo
}
void drawRoofAndFloorInRows() {
    Uint32* pixels = (Uint32*)backBuffer->pixels;
    int width = backBuffer->w;
    int height = backBuffer->h;
    int halfPoint = width * height / 2;

    std::fill(pixels, pixels + halfPoint, ceilingColour);
    std::fill(pixels + halfPoint, pixels + width * height, floorColour);
}

//temp
void make_dot(int x) {
    setPixel(x, Settings::getScreenHeight()/2, 0xFF0000FF);
    setPixel(x, Settings::getScreenHeight()/2 +1, 0xFF0000FF);
    setPixel(x, Settings::getScreenHeight()/2 +2, 0xFF0000FF);
    setPixel(x, Settings::getScreenHeight()/2 +3, 0xFF0000FF);
    setPixel(x, Settings::getScreenHeight()/2 +4, 0xFF0000FF);
    setPixel(x, Settings::getScreenHeight()/2 +5, 0xFF0000FF);
    setPixel(x, Settings::getScreenHeight()/2 +6, 0xFF0000FF);
    setPixel(x, Settings::getScreenHeight()/2 +7, 0xFF0000FF);
    setPixel(x +1, Settings::getScreenHeight()/2, 0xFF0000FF);
    setPixel(x +1, Settings::getScreenHeight()/2 +1, 0xFF0000FF);
    setPixel(x +1, Settings::getScreenHeight()/2 +2, 0xFF0000FF);
    setPixel(x +1, Settings::getScreenHeight()/2 +3, 0xFF0000FF);
    setPixel(x +1, Settings::getScreenHeight()/2 +4, 0xFF0000FF);
    setPixel(x +1, Settings::getScreenHeight()/2 +5, 0xFF0000FF);
    setPixel(x +1, Settings::getScreenHeight()/2 +6, 0xFF0000FF);
    setPixel(x +1, Settings::getScreenHeight()/2 +7, 0xFF0000FF);
    setPixel(x -1, Settings::getScreenHeight()/2, 0xFF0000FF);
    setPixel(x -1, Settings::getScreenHeight()/2 +1, 0xFF0000FF);
    setPixel(x -1, Settings::getScreenHeight()/2 +2, 0xFF0000FF);
    setPixel(x -1, Settings::getScreenHeight()/2 +3, 0xFF0000FF);
    setPixel(x -1, Settings::getScreenHeight()/2 +4, 0xFF0000FF);
    setPixel(x -1, Settings::getScreenHeight()/2 +5, 0xFF0000FF);
    setPixel(x -1, Settings::getScreenHeight()/2 +6, 0xFF0000FF);
    setPixel(x -1, Settings::getScreenHeight()/2 +7, 0xFF0000FF);
}

static void drawWallColumn(int x, int wallHeight, double xPosWithinTexture, int wallNum) {

    int screenHeight = Settings::getScreenHeight();
    int unboundedTop = (screenHeight-wallHeight) /2;
    int unboundedBottom = unboundedTop + wallHeight;

    /*for (int i=0; i < unboundedTop; i++) {
        setPixel(x, i, floorColour);
    }*/

    int textureHeight = wallTextures[wallNum]->getHeight();
    //const Uint32* column = wallTextures[wallNum]->getColumn(xPosWithinTexture);
    
    
    double pixel_gap = static_cast<double>(textureHeight) / wallHeight;
    int top = std::max(unboundedTop, 0);
    int bottom = std::min(unboundedBottom, screenHeight);

    drawTexturedColumn(x, unboundedTop, unboundedBottom, xPosWithinTexture, 1, wallNum, pixel_gap);

    /*for (int i=bottom; i < screenHeight; i++) {
        setPixel(x, i, ceilingColour);
    }*/
}
void drawSpriteColumn(int x, int spriteHeight, double xPosWithinTexture, int spriteNum) {//need to account for fact that sprites are not in middle of screen
    int screenHeight = Settings::getScreenHeight();
    int unboundedTop = (screenHeight-spriteHeight) /2;
    int unboundedBottom = unboundedTop + spriteHeight; //this should only be calcuated once in the outer function. Maybe.

    int textureHeight = spriteTextures[spriteNum]->getHeight();
    const Uint32* column = spriteTextures[spriteNum]->getColumn(xPosWithinTexture);
    double pixel_gap = static_cast<double>(textureHeight) / spriteHeight;

    int top = std::max(unboundedTop, 0);
    int bottom = std::min(unboundedBottom, screenHeight);

    drawTexturedColumn(x, unboundedTop, unboundedBottom, xPosWithinTexture, 0, 0, pixel_gap);

    /*for (int i=top; i < bottom; i++) {
        int textureY = std::round((i-unboundedTop)*pixel_gap);
        
        //assert(textureY >= 0);
        //if (textureY >= static_cast<int>(column.size())) {// clamp textureY to within the texture
        //    textureY = static_cast<int>(column.size() - 1);// it fixes black lines
        //}
        setPixel(x, i, column[textureY]);
    }*/
}
void drawSprite(Sprite* sprite, int spriteCenterScreenColumn, int spriteScreenSize, double* rayLengths, double spriteDistance) {
    int temp = spriteScreenSize / 3;// dont use this should probably be based on the images width height ratio

    double xPosWithinTexture = 0;
    for (int i=-temp; i<temp; i++) {// switch to iterating from 0

        int where = spriteCenterScreenColumn+i;
        if (where >= 0 && where < Settings::getScreenWidth()) {//avoid drawing sprites off screen

            //std::cout << "spriteCameraDistance " << spriteCameraDistance << " rayLengths[i+temp] " << rayLengths[i+temp] << std::endl;
            //std::cout << "i+temp " << i+temp << std::endl;
            //std::cout << "comparison" << (spriteCameraDistance < rayLengths[i+temp]) << std::endl;
            if (spriteDistance < rayLengths[spriteCenterScreenColumn+i]) {
                drawSpriteColumn(spriteCenterScreenColumn+i, spriteScreenSize, xPosWithinTexture, 0);//still not actually picking the correct sprite texturev
            }
        }
        xPosWithinTexture = xPosWithinTexture + (1.0 / spriteScreenSize);
    }
}

double calculateBearing(double x1,double y1, std::tuple<double, double> point2) {
    double x2 = std::get<0>(point2);
    double y2 = std::get<1>(point2);

    double deltaX = x2 - x1;
    double deltaY = y2 - y1;

    double bearing = (atan2(deltaX, deltaY) - M_PI_2) * -1;
    return fmod(bearing + 2 * M_PI, 2 * M_PI);
}
bool isInViewFrustum(double playerX, double playerY, double playerRotation, int spriteCenterScreenColumn, Sprite* sprite, int spriteScreenSize) {
    // Appears to make no measureable performance difference

    int temp = spriteScreenSize / 3;// dont use this magic value finalise

    double spriteCenterBearing = calculateBearing(playerX, playerY, sprite->getPosition());
    // is more left or right of player view
    if (spriteCenterBearing > playerRotation) {// it doesnt seem completely acurate but still good enough I dont know why
        return spriteCenterScreenColumn - temp < Settings::getScreenWidth();
    } else {
        return spriteCenterScreenColumn + temp > 0;
    }
}

void drawFrame(double positionX, double positionY, double rotation) {

    drawRoofAndFloorInRows();

    int sWidth = Settings::getScreenWidth();
    double rayLengths[sWidth];// very naughty VLA

    // raycast the whole screen and draw walls
    double degreesPerPixel = Settings::getFov() / Settings::getScreenWidth();
    double raycastAngle = rotation - (Settings::getFov()/2);// starting value for leftmost column
    for (int i=0; i < Settings::getScreenWidth(); i++) {

        auto [distance, side, xPosWithinTexture, wallnum] = raycast(positionX, positionY, raycastAngle);

        rayLengths[i] = distance;// store for later sprite drawing
        
        distance = distance * cos(raycastAngle - rotation);// fish eye effect corrections

        int wallHeight = (int)(1.5 * Settings::getScreenHeight() / distance);
 
        raycastAngle = raycastAngle + degreesPerPixel;

        drawWallColumn(i, wallHeight, xPosWithinTexture, wallnum);
    }
    // Sort sprites by distance so they render correctly
    std::sort(sprites.begin(), sprites.end(), [positionX, positionY](const Sprite* a, const Sprite* b) {
        return a->getSquaredDistanceFrom(positionX, positionY) >
                b->getSquaredDistanceFrom(positionX, positionY);
    });


    // Draw Sprites
    for (Sprite* sprite : sprites) {//need to later do this sorted by distance
        double spriteDistance = sprite->getDistanceFrom(positionX, positionY);
        int spriteScreenSize = (int)(1.5 * Settings::getScreenHeight() / spriteDistance);
        double spriteAngle = rotation - calculateBearing(positionX,positionY,sprite->getPosition());
        //double calcB = calculateBearing(positionX,positionY,sprite->getPosition());
        //std::println("player rotation {}", rotation*(180/M_PI));
        //std::println("enemy bearing {}", calcB*(180/M_PI));
        //std::println("- {}", spriteAngle);
        //std::println("spriteAngle {}", spriteAngle*(180/M_PI));

        int spriteCenterScreenColumn = (Settings::getScreenWidth() /2) - (spriteAngle / degreesPerPixel);

        if (isInViewFrustum(positionX, positionY, rotation, spriteCenterScreenColumn, sprite, spriteScreenSize)) {
            drawSprite(sprite, spriteCenterScreenColumn, spriteScreenSize, rayLengths, spriteDistance);
        }
        //make_dot(spriteCenterScreenColumn);
        
    }
    //crosshair
    //setPixel(Settings::getScreenWidth()/2, Settings::getScreenHeight()/2,0xFFFF0000);
}
