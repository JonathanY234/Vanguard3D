#include <iostream>// doubt this is needed
#include <print>
//#include <cstdlib> // For system()
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

static std::vector<Sprite> sprites;
void initialiseSprites() {
    getSprites(sprites);
    //for (int val : sprites) {
    //    std::cout << val << " ";
    //}
}


// function to set the colour of the screen, no bounds checking, uses the global backbuffer
// frequently used so needs to be performant
static void setPixel(int x, int y, Uint32 colour) {
    Uint32* pixels = (Uint32*)backBuffer->pixels;
    pixels[(y * backBuffer->w) + x] = colour;
} //make a super optimised row drawing setPixel

static void drawColumn(int x, int wallHeight, double xPosWithinTexture, int wallNum) {

    int screenHeight = Settings::getScreenHeight();
    int floorTop = (screenHeight-wallHeight) /2;
    int wallTop = floorTop + wallHeight;

    for (int i=0; i < floorTop; i++) {
        setPixel(x, i, floorColour);
    }

    const std::vector<Uint32>& column = wallTextures[wallNum]->getColumn(xPosWithinTexture);
    double pixel_gap = 100.0 / wallHeight;// why is this still hardcoded

    int top = std::max(floorTop, 0);
    int bottom = std::min(wallTop, screenHeight);
    for (int i=top; i < bottom; i++) {
        int textureY = std::round((i-floorTop)*pixel_gap);//    also experiment with more "up" taller walls
        
        assert(textureY >= 0);
        if (textureY >= static_cast<int>(column.size())) {// clamp textureY to within the texture
            textureY = static_cast<int>(column.size() - 1);// it fixes black lines
        }
        setPixel(x, i, column[textureY]);
    }

    for (int i=wallTop; i < screenHeight; i++) {
        setPixel(x, i, ceilingColour);
    }
}
void drawSpriteColumn(int x, int spriteHeight, double xPosWithinTexture, int spriteNum) {//need to account for fact that strites are not in middle of screen
    int screenHeight = Settings::getScreenHeight();
    int unboundedTop = (screenHeight-spriteHeight) /2;
    int unboundedBottom = unboundedTop + spriteHeight; //this should only be calcuated once in the main renderer function. Maybe.

    const std::vector<Uint32>& column = spriteTextures[spriteNum]->getColumn(xPosWithinTexture);
    double pixel_gap = 100.0 / spriteHeight;// why is this still hardcoded

    //int top = std::max(unboundedTop, 0);
    //int bottom = std::min(unboundedBottom, screenHeight);
    //no of the top of screen bounds protections??

    int top = std::max(unboundedTop, 0);
    int bottom = std::min(unboundedBottom, screenHeight);
    for (int i=top; i < bottom; i++) {
        int textureY = std::round((i-unboundedTop)*pixel_gap);//    also experiment with more "up" taller walls
        
        //assert(textureY >= 0);
        //if (textureY >= static_cast<int>(column.size())) {// clamp textureY to within the texture
        //    textureY = static_cast<int>(column.size() - 1);// it fixes black lines
        //}
        setPixel(x, i, column[textureY]);
    }
}
double calculateBearing(double x1,double y1, std::tuple<double, double> point2) {
    double x2 = std::get<0>(point2);
    double y2 = std::get<1>(point2);

    double deltaX = x2 - x1;
    double deltaY = y2 - y1;

    double bearing = fmod(atan2(deltaX, deltaY) + 2 * M_PI, 2 * M_PI);
    return bearing;
}

//static double rayLengths[Settings::getScreenWidth];
void drawFrame(double positionX, double positionY, double rotation) {

    int sWidth = Settings::getScreenWidth();
    double rayLengths[sWidth];  // very naughty VLA

    // raycast the whole screen and draw walls
    double degreesPerPixel = Settings::getFov() / Settings::getScreenWidth();
    double raycastAngle = rotation - (Settings::getFov()/2); // starting value for leftmost column
    for (int i=0; i < Settings::getScreenWidth(); i++) {

        auto [distance, side, xPosWithinTexture, wallnum] = raycast(positionX, positionY, raycastAngle);

        rayLengths[i] = distance;// store for later sprite drawing
        
        distance = distance * cos(raycastAngle - rotation); // fish eye effect corrections

        int wallHeight = (int)(1.5 * Settings::getScreenHeight() / distance);
 
        raycastAngle = raycastAngle + degreesPerPixel;

        drawColumn(i, wallHeight, xPosWithinTexture, wallnum);
    }
    //std::cout << sprites << std::endl;
    for (Sprite sprite : sprites) {//need to later do this sorted by distance
        double spriteDistance = sprite.getDistanceFrom(positionX, positionY);
        // what pixel column on the screen is the center of the sprite
        int spriteScreenSize = (int)(1.5 * Settings::getScreenHeight() / spriteDistance);
        double calcB = calculateBearing(positionX,positionY,sprite.getPosition());
        //double spriteAngle = rotation - calculateBearing(positionX,positionY,sprite.getPosition());
        double spriteAngle = rotation - calcB;
        int spriteCenterScreenColumn = (Settings::getScreenWidth() /2) + (spriteAngle * degreesPerPixel);

        setPixel(spriteCenterScreenColumn, Settings::getScreenHeight()/2, 0xFFFF0000);
        setPixel(spriteCenterScreenColumn, Settings::getScreenHeight()/2 +1, 0xFFFF0000);
        setPixel(spriteCenterScreenColumn, Settings::getScreenHeight()/2 +2, 0xFFFF0000);
        setPixel(spriteCenterScreenColumn, Settings::getScreenHeight()/2 +3, 0xFFFF0000);
        setPixel(spriteCenterScreenColumn, Settings::getScreenHeight()/2 +4, 0xFFFF0000);
        //std::cout << "SCSC" <<  spriteCenterScreenColumn << std::endl;
        //for (int i=)
        
    }//TODO: 

    //crosshair
    //setPixel(Settings::getScreenWidth()/2, Settings::getScreenHeight()/2,0xFFFF0000);
}

