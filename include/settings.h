#ifndef SETTIGNS_H
#define SETTINGS_H

#include <SDL.h>

class Settings {
private:
    static int screenHeight;
    static int screenWidth;
    static int fps;
public:
    static int getScreenWidth();
    static int getScreenHeight();
    static int getFps();
    static int getFrameTime();
};


#endif // SETTINGS_H