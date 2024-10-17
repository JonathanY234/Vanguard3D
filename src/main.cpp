#include <SDL.h>
#include <iostream>
#include <tuple>

#include "settings.h"
#include "renderer.h"
#include "game.h"


// Define the global backBuffer
SDL_Surface* backBuffer = nullptr;


int main() {
    // data for frame rate cap
    Uint32 currentFrameStart;
    double actualFrameTime;
    // data for deltaTime
    double deltaTime;
    Uint32 previousFrameStart = SDL_GetTicks();

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
    std::shared_ptr<Player> firstPlayer = std::make_shared<Player>(2, 2, 0);
    // oh fancy smart pointer

    // Main loop to keep the window open
    bool running = true;
    SDL_Event event;
    const Uint8* keyboardState;

    while (running) {
        // get deltatime and frame limiter
        currentFrameStart = SDL_GetTicks();
        deltaTime = (currentFrameStart - previousFrameStart) / 1000.0; // Convert to seconds
        previousFrameStart = currentFrameStart;

        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false; // Exit the loop if the window is closed
            }
        }
        // Get the current state of the keyboard
        keyboardState = SDL_GetKeyboardState(NULL);
        // Check for specific keys
        if (keyboardState[SDL_SCANCODE_W]) {
            firstPlayer->moveForward(5 * deltaTime);
        }
        if (keyboardState[SDL_SCANCODE_S]) {
            firstPlayer->moveForward(-5 * deltaTime);
        }
        if (keyboardState[SDL_SCANCODE_A]) {
            firstPlayer->moveSideways(-3 * deltaTime);
        }
        if (keyboardState[SDL_SCANCODE_D]) {
            firstPlayer->moveSideways(3 * deltaTime);
        }
        if (keyboardState[SDL_SCANCODE_LEFT]) {
            firstPlayer->turn(-1.5 * deltaTime);
        }
        if (keyboardState[SDL_SCANCODE_RIGHT]) {
            firstPlayer->turn(1.5 * deltaTime);
        }

        // Clear the back buffer with black color
        //SDL_FillRect(backBuffer, NULL, SDL_MapRGB(backBuffer->format, 0, 0, 0));

        drawFrame(firstPlayer); // meybe just through location and direction into drawframe not the whole object

        // Copy back buffer to the front buffer
        // swap pointers for efficienct double buffering
        SDL_Surface* temp = frontBuffer;
        frontBuffer = backBuffer;
        backBuffer = temp;

        // Update the window surface to display the front buffer
        SDL_UpdateWindowSurface(window);

        actualFrameTime = SDL_GetTicks() - currentFrameStart; // Calculate how long the frame took
        // Delay to maintain target FPS
        if (actualFrameTime < Settings::getFrameTime()) {
            SDL_Delay(Settings::getFrameTime() - actualFrameTime);
        }
    }

    // Clean up
    SDL_FreeSurface(backBuffer);
    SDL_FreeSurface(frontBuffer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
