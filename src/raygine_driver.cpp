#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include "../include/raygine_renderer.hpp"

const int window_width = 800;
const int window_height = 600;
// const int cell_size = 20;
const int cell_size = 60;
const float PI = 3.14159265359;

void InitSDL();
float degree_to_rad(float &in_degree)
{
    if (in_degree > 360)
    {
        in_degree -= 360;
    }
    else if (in_degree < 0)
    {
        in_degree += 360;
    }
    return (in_degree) * (PI / 180);
}





// std::vector<std::vector<int>> map = {
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1},
//     {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1},
//     {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
//     {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
//     {1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//     {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
// };

std::vector<std::vector<int>> map = {
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1}
};




void draw_map()
{
    for (int y = 0; y < map.size(); ++y)
    {
        for (int x = 0; x < map[y].size(); ++x)
        {
            SDL_Rect r = { x * cell_size, y * cell_size, cell_size, cell_size };
            if (map[y][x] == 1)
            {
                RaygineRenderer::SetDrawColor(255, 255, 255, 255);
                SDL_RenderFillRect(RaygineRenderer::GetRenderer(), &r);
            }
            // draw border.
            RaygineRenderer::SetDrawColor(120, 120, 120, 255);
            SDL_RenderDrawRect(RaygineRenderer::GetRenderer(), &r);
        }
    }
}

void draw_player(float player_x, float player_y, float player_dir_x, float player_dir_y)
{
    SDL_FRect player_r = { player_x, player_y, 5, 5 };
    RaygineRenderer::SetDrawColor(255, 50, 0, 255);
    SDL_RenderFillRectF(RaygineRenderer::GetRenderer(), &player_r);

    // Calculate the end point of the direction line
    float line_length = 20.0f; // Length of the direction line
    float end_x = player_x + player_dir_x * line_length;
    float end_y = player_y + player_dir_y * line_length;

    // Draw the direction line
    SDL_SetRenderDrawColor(RaygineRenderer::GetRenderer(), 0, 255, 0, 255); // Green color for the direction line
    SDL_RenderDrawLineF(RaygineRenderer::GetRenderer(), player_x, player_y, end_x, end_y);
}



int main()
{
    float player_pos_x = 145.0f;
    float player_pos_y = 85.0f;
    float player_delta_x = 0.0f;
    float player_delta_y = 0.0f;
    float player_angle = 0.0f;

    InitSDL();
    RaygineRenderer::InitWindow(window_width, window_height);
    RaygineRenderer::CreateRenderer();
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
                    case SDLK_w: // Move forward
                    {
                        player_pos_x += player_delta_x * 1.5;
                        player_pos_y += player_delta_y * 1.5;
                    }
                    break;
                    case SDLK_s: // Move backward
                    {
                        player_pos_x -= player_delta_x * 1.5;
                        player_pos_y -= player_delta_y * 1.5;
                    }
                    break;
                    case SDLK_a: // Rotate left (counter-clockwise)
                    {
                        player_angle += 5; // Adjust rotation speed as needed
                        if (player_angle >= 360) 
                        {
                            player_angle -= 360;
                        }
                        player_delta_x = cos(degree_to_rad(player_angle));
                        player_delta_y = -sin(degree_to_rad(player_angle));
                    }
                    break;
                    case SDLK_d:  // Rotate right (clockwise)
                    {
                        player_angle -= 5; // Adjust rotation speed as needed
                        if (player_angle < 0) 
                        {
                            player_angle += 360;
                        }
                        player_delta_x = cos(degree_to_rad(player_angle));
                        player_delta_y = -sin(degree_to_rad(player_angle));
                    }
                    break;
                }
            }
        }
        // update
        // render
        RaygineRenderer::SetDrawColor(0, 0, 0, 255);
        RaygineRenderer::ClearRenderer();
        draw_map();
        draw_player(player_pos_x, player_pos_y, player_delta_x, player_delta_y);
        std::cout << "x: " << player_pos_x << ", y: " << player_pos_y << " " << player_angle << "\n";
        SDL_RenderPresent(RaygineRenderer::GetRenderer());
    }
    
    RaygineRenderer::DestroyRender();
    RaygineRenderer::DestroyWindow();
    SDL_Quit();
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