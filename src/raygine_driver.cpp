#include <iostream>
#include <SDL2/SDL.h>
#include "../include/raygine_renderer.hpp"

void InitSDL();

const int window_width = 800;
const int window_height = 600;

int main()
{
    InitSDL();
    RaygineRenderer::InitWindow(window_width, window_height);
    std::cout << "Hello, from Raygine!" << std::endl;
    // todo, make input manager.
    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
    }
    
    RaygineRenderer::DestroyWindow();
    return 0;
}

void InitSDL()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cerr << "SDL failed it init!\n";
        exit(1);
    }
}