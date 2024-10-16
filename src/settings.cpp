#include "settings.h"
#include <cmath>


int Settings::screenHeight = 600;
int Settings::screenWidth = 800;
int Settings::fps = 60;
double Settings::fov = 90;

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
