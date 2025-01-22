#include <SDL.h>
#include <iostream>
#include <print>
#include <tuple>

#include "settings.h"
#include "renderer.h"
#include "game.h"

#include "load_textures.h"
#include "level.h"

// Define the global backBuffer
SDL_Surface* backBuffer = nullptr;

int main() {
    loadTextures();
    initialiseSprites();

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1; // Exit if initialization fails
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow(
        "Vanguard 3D",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        Settings::getScreenWidth(), Settings::getScreenHeight(),
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1; // Exit if window creation fails
    }

    // Create two surfaces for double buffering
    backBuffer = SDL_CreateRGBSurface(0, Settings::getScreenWidth(), Settings::getScreenHeight(), 32,
        0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    
    SDL_Surface* frontBuffer = SDL_GetWindowSurface(window);

    if (!backBuffer) {
        std::cout << "Back buffer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1; // Exit if back buffer creation fails
    }

    // make the player object
    std::shared_ptr<Player> firstPlayer = std::make_shared<Player>(0.5, 0.5, 0.0, 0.35);
    // ooh fancy smart pointer

    //mouselook stuff
    SDL_SetRelativeMouseMode(SDL_TRUE);

    // Main loop to keep the window open
    bool running = true;
    SDL_Event event;
    
    // frame timing and cap stuff
    Uint32 previousFrameStart = SDL_GetTicks();
    Uint32 frameRateTestStart = SDL_GetTicks();
    int frameCount = 0;

    while (running) {
        // get deltatime and frame limiter
        Uint32 currentFrameStart = SDL_GetTicks();
        double deltaTime = (currentFrameStart - previousFrameStart) / 1000.0; // convert to seconds
        previousFrameStart = currentFrameStart;

        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false; // exit if the window is closed
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false; // exit on ESC key
                }
            }
        }
        // Get the current state of the keyboard
        const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
        // Check for specific keys

        // mouse look
        int mouseDeltaX;
        SDL_GetRelativeMouseState(&mouseDeltaX, nullptr);
        firstPlayer->turn(mouseDeltaX * 0.004);
        // arrow keys look
        if (keyboardState[SDL_SCANCODE_LEFT]) {
            firstPlayer->turn(-1.5 * deltaTime);
        }
        if (keyboardState[SDL_SCANCODE_RIGHT]) {
            firstPlayer->turn(1.5 * deltaTime);
        }
        // player movement
        double forwardAmount = 0.0;
        double sidewaysAmount = 0.0;
        if (keyboardState[SDL_SCANCODE_W]) {
            forwardAmount += 5;
        }
        if (keyboardState[SDL_SCANCODE_S]) {
            forwardAmount -= 5;
        }
        if (keyboardState[SDL_SCANCODE_A]) {
            sidewaysAmount -= 3;
        }
        if (keyboardState[SDL_SCANCODE_D]) {
            sidewaysAmount += 3;
        }
        firstPlayer->move(forwardAmount * deltaTime, sidewaysAmount * deltaTime);
        

        // Clear the back buffer with black color (not nessesary)
        //SDL_FillRect(backBuffer, NULL, SDL_MapRGB(backBuffer->format, 0, 0, 0));

        auto [x, y] = firstPlayer->getPosition();
        drawFrame(x, y, firstPlayer->getRotation());

        // Copy back buffer to the front buffer
        // swap pointers for efficienct double buffering
        SDL_Surface* temp = frontBuffer;
        frontBuffer = backBuffer;
        backBuffer = temp;

        // Update the window surface to display the front buffer
        SDL_UpdateWindowSurface(window);

        //should be improved with SDL_GetPerformanceCounter() for more accuracy to avoid the 62 fps problem
        double actualFrameTime = SDL_GetTicks() - currentFrameStart; // Calculate how long the frame took
        // Delay to maintain target FPS
        if (actualFrameTime < Settings::getFrameTime()) {
            SDL_Delay(Settings::getFrameTime() - actualFrameTime);
        }        

        // Print FPS every x frames
        frameCount++;
        int n = 100;
        if (frameCount == n) {
            double avgFrameTime = static_cast<double>(SDL_GetTicks() - frameRateTestStart) / n;
            double avgFrameRate = 1000.0 / avgFrameTime;
            std::println("{}", avgFrameTime);
            std::println("{} {}", "avg FPS:", avgFrameRate);

            frameCount = 0;
            frameRateTestStart = SDL_GetTicks();
        }
    
 
    }

    // Clean up
    SDL_FreeSurface(backBuffer);
    SDL_FreeSurface(frontBuffer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
