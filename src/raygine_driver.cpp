#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include "../include/raygine_renderer.hpp"

void InitSDL();

const int window_width = 800;
const int window_height = 600;

std::vector<std::vector<int>> map = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};


const int cell_size = 20;

void draw_map()
{
    for (int y = 0; y < map.size(); ++y)
    {
        for (int x = 0; x < map[y].size(); ++x)
        {
            if (map[y][x] == 1)
            {
                SDL_Rect r = { x * cell_size, y * cell_size, cell_size, cell_size };
                RaygineRenderer::SetDrawColor(255, 255, 255, 255);
                SDL_RenderFillRect(RaygineRenderer::GetRenderer(), &r);
            }
        }
    }
}

int main()
{
    float player_x = 0.0f;
    float player_y = 0.0f;
    SDL_Rect me = {

    };
    InitSDL();
    RaygineRenderer::InitWindow(window_width, window_height);
    RaygineRenderer::CreateRenderer();
    std::cout << "Hello, from Raygine!" << std::endl;
    draw_map();
    // todo, make input manager.
    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        // input
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                    case SDLK_w:
                        player_y -= 5;
                        break;
                    case SDLK_s:
                        player_y += 5;
                        break;
                    case SDLK_a:
                        player_x -= 5;
                        break;
                    case SDLK_d:
                        player_x += 5;
                        break;
                }
            }
        }
        // update
        // render
        RaygineRenderer::SetDrawColor(0, 0, 0, 255);
        RaygineRenderer::ClearRenderer();
        // RaygineRenderer::SetDrawColor(255, 255, 255, 255);
        // SDL_Rect r = {
        //     static_cast<int>(player_x),
        //     static_cast<int>(player_y),
        //     20,
        //     20
        // };
        // SDL_RenderFillRect(RaygineRenderer::GetRenderer(), &r);
        draw_map();
        SDL_RenderPresent(RaygineRenderer::GetRenderer());
    }
    
    RaygineRenderer::DestroyRender();
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