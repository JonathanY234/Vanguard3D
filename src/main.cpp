#include <SDL.h>
#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int targetFPS = 60;
const int frameDelay = 1000 / targetFPS; // Time in milliseconds for each frame
Uint32 frameStart;
int frameTime;


// Function to set a pixel on the surface
void setPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
    Uint32* pixels = (Uint32*)surface->pixels;
    pixels[(y * surface->w) + x] = color;
}

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1; // Exit if initialization fails
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow(
        "Vanguard 3D",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1; // Exit if window creation fails
    }

    // Create two surfaces for double buffering
    SDL_Surface* backBuffer = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
        0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    
    SDL_Surface* frontBuffer = SDL_GetWindowSurface(window);

    if (!backBuffer) {
        std::cout << "Back buffer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1; // Exit if back buffer creation fails
    }

    // Main loop to keep the window open
    bool running = true;
    SDL_Event event;

    while (running) {
        frameStart = SDL_GetTicks(); // Get the current time

        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false; // Exit the loop if the window is closed
            }
        }

        // Clear the back buffer with black color
        //SDL_FillRect(backBuffer, NULL, SDL_MapRGB(backBuffer->format, 0, 0, 0));

        // Draw something on the back buffer (example: a simple gradient)
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            for (int x = 0; x < SCREEN_WIDTH; x++) {
                // Create a gradient effect
                Uint32 color = SDL_MapRGB(backBuffer->format, x % 256, y % 256, 0);
                setPixel(backBuffer, x, y, color);
            }
        }

        // Copy back buffer to the front buffer
        // swap pointers for efficienct double buffering
        SDL_Surface* temp = frontBuffer;
        frontBuffer = backBuffer;
        backBuffer = temp;
        // SDL_BlitSurface(backBuffer, NULL, frontBuffer, NULL); -old slow blitting method 

        // Update the window surface to display the front buffer
        SDL_UpdateWindowSurface(window);

        frameTime = SDL_GetTicks() - frameStart; // Calculate how long the frame took
        // Delay to maintain target FPS
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    // Clean up
    SDL_FreeSurface(backBuffer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
