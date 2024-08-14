#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
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

void draw_player(float player_x, float player_y, float player_dir_x, float player_dir_y)
{
    SDL_FRect player_r = { player_x, player_y, 5, 5 };
    RaygineRenderer::SetDrawColor(255, 50, 0, 255);
    SDL_RenderFillRectF(RaygineRenderer::GetRenderer(), &player_r);

    // Calculate the end point of the direction line
    // float line_length = 20.0f; // Length of the direction line
    // float end_x = player_x + player_dir_x * line_length;
    // float end_y = player_y + player_dir_y * line_length;

    // // Draw the direction line
    // SDL_SetRenderDrawColor(RaygineRenderer::GetRenderer(), 0, 255, 0, 255); // Green color for the direction line
    // SDL_RenderDrawLineF(RaygineRenderer::GetRenderer(), player_x, player_y, end_x, end_y);
}

void cast_ray(float player_x, float player_y, float dir_x, float dir_y) {
    int mapX = static_cast<int>(player_x / cell_size);
    int mapY = static_cast<int>(player_y / cell_size);

    float rayDirX = dir_x;
    float rayDirY = dir_y;

    float deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
    float deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);

    int stepX, stepY;
    float sideDistX, sideDistY;

    if (rayDirX < 0) {
        stepX = -1;
        sideDistX = (player_x - mapX * cell_size) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (mapX * cell_size + cell_size - player_x) * deltaDistX;
    }
    if (rayDirY < 0) {
        stepY = -1;
        sideDistY = (player_y - mapY * cell_size) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (mapY * cell_size + cell_size - player_y) * deltaDistY;
    }

    bool hit = false;
    int side;
    while (!hit) {
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            mapX += stepX;
            side = 0;
        } else {
            sideDistY += deltaDistY;
            mapY += stepY;
            side = 1;
        }
        if (map[mapY][mapX] > 0) hit = true;
    }

    float perpWallDist;
    if (side == 0)
        perpWallDist = (mapX * cell_size - player_x + (1 - stepX) * cell_size / 2) / rayDirX;
    else
        perpWallDist = (mapY * cell_size - player_y + (1 - stepY) * cell_size / 2) / rayDirY;

    float impactX = player_x + rayDirX * perpWallDist;
    float impactY = player_y + rayDirY * perpWallDist;

    SDL_SetRenderDrawColor(RaygineRenderer::GetRenderer(), 0, 255, 0, 255);
    SDL_RenderDrawLineF(RaygineRenderer::GetRenderer(), player_x, player_y, impactX, impactY);
}



int main()
{
    float player_pos_x = 145.0f;
    float player_pos_y = 85.0f;
    float player_dir_x = 0.0f;
    float player_dir_y = 1.0f;
    const float rotation_speed = 0.1f;

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
                    case SDLK_w: // Move forward
                        player_pos_x += player_dir_x * 1.5;
                        player_pos_y += player_dir_y * 1.5;
                        break;
                    case SDLK_s: // Move backward
                        player_pos_x -= player_dir_x * 1.5;
                        player_pos_y -= player_dir_y * 1.5;
                        break;
                    case SDLK_a: // Rotate left (counter-clockwise)
                        {
                            float old_dir_x = player_dir_x;
                            player_dir_x = player_dir_x * cos(-rotation_speed) - player_dir_y * sin(-rotation_speed);
                            player_dir_y = old_dir_x * sin(-rotation_speed) + player_dir_y * cos(-rotation_speed);
                        }
                        break;
                    case SDLK_d: // Rotate right (clockwise) <- from a rotation matrix.
                        {
                            float old_dir_x = player_dir_x;
                            player_dir_x = player_dir_x * cos(rotation_speed) - player_dir_y * sin(rotation_speed);
                            player_dir_y = old_dir_x * sin(rotation_speed) + player_dir_y * cos(rotation_speed);
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
        draw_player(player_pos_x, player_pos_y, player_dir_x, player_dir_y);
        std::cout << "x: " << player_pos_x << ", y: " << player_pos_y << ", x_dir: " << player_dir_x << ", y_dir: " << player_dir_y << "\n";
        cast_ray(player_pos_x, player_pos_y, player_dir_x, player_dir_y);
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