#include "../include/raygine_renderer.hpp"
void RaygineRenderer::InitWindow(int window_width, int window_height)
{
    int window_x = SDL_WINDOWPOS_CENTERED;
    int window_y = SDL_WINDOWPOS_CENTERED;
    SDL_Window* window = SDL_CreateWindow(
        _raygine_window_name,       
        window_x,       
        window_y,    
        window_width,      
        window_height,      
        SDL_WINDOW_SHOWN    
    );

    if (window == nullptr)
    {
        std::cerr << "Failed to create SDL window using SDL_CreateWindow()\n";
        exit(1);
    }

    _raygine_window = window;
}

void RaygineRenderer::DestroyWindow()
{
    SDL_DestroyWindow(_raygine_window);
}