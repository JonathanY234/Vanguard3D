#include "settings.h"
#include <cmath>


int Settings::screenHeight = 1000;// idea: allow running at low res and scaling
int Settings::screenWidth = 1800;
int Settings::fps = 60; // could set this value based on sdl functions to get refresh rate
double Settings::fov = 60;
// add fov adjustions for aspect ratio

int Settings::getScreenHeight() {
    return screenHeight;
}
int Settings::getScreenWidth() {
    return screenWidth;
}
int Settings::getFps() {
    return fps;
}
int Settings::getFrameTime() {
    return 1000/fps;
}
double Settings::getFov() {
    // convert to radians
    return fov * (M_PI / 180);;
}
