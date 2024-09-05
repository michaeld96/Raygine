#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include "../include/raygine_renderer.hpp"
#include <utility> // For std::pair.
#include <limits>
#include <algorithm> // For std::clamp
#include "../include/vec2.hpp"

#define DEBUG

const int window_width = 800;
const int window_height = 400;
const int cell_size = 50;
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
std::pair<int, int> ray_to_map_coordinates(float ray_x, float ray_y, int unit_size)
{
    int int_ray_x = static_cast<int>(ray_x) / unit_size;
    int int_ray_y = static_cast<int>(ray_y) / unit_size;
    return std::make_pair(int_ray_x, int_ray_y);
}

float MakeInBounds(float _in)
{
    if (_in > 360)
    {
        _in -= 360;
    }
    else if (_in < 0)
    {
        _in += 360;
    }
    return _in;
}

std::vector<std::vector<int>> map = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}
};

float distance_formula(float x1, float y1, float x2, float y2)
{
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}


typedef struct {
    Vec2<float> pos;
    Vec2<float> dir;
} Player;




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
    RaygineRenderer::SetDrawColor(255, 0, 0, 255);
    SDL_RenderFillRectF(RaygineRenderer::GetRenderer(), &player_r);

    float line_length = 20.0f; // Length of the direction line.
    float end_x = player_x + player_dir_x * line_length;
    float end_y = player_y + player_dir_y * line_length;

    // Draw the direction line.
    SDL_SetRenderDrawColor(RaygineRenderer::GetRenderer(), 255, 0, 0, 255);
    SDL_RenderDrawLineF(RaygineRenderer::GetRenderer(), player_x, player_y, end_x, end_y);
}

// void DrawRay(float player_x, float player_y, float dx, float dy, float player_angle, Player* player)
// {
//     Vec2<float> ray = {
//         player->dir.x,
//         player->dir.y
//     };

//     // Get which map cell we are in.
//     int map_x = static_cast<int>(player_x) / cell_size;
//     int map_y = static_cast<int>(player_y) / cell_size;

//     float vertical_x_dist;
//     float horizontal_y_dist;

//     // Calculate the step and initial distance to the first vertical and horizontal grid lines.
//     float delta_dist_x = (ray.x == 0) ? 1e30 : std::abs(1 / ray.x);
//     float delta_dist_y = (ray.y == 0) ? 1e30 : std::abs(1 / ray.y);

//     int step_x, step_y;

//     // Determine step direction and initial side distance
//     if (ray.x < 0) // Moving left
//     {
//         step_x = -1;
//         vertical_x_dist = (player_x - map_x * cell_size) * delta_dist_x;
//     }
//     else // Moving right
//     {
//         step_x = 1;
//         vertical_x_dist = ((map_x + 1) * cell_size - player_x) * delta_dist_x;
//     }
    
//     if (ray.y < 0) // Moving down
//     {
//         step_y = -1;
//         horizontal_y_dist = (player_y - map_y * cell_size) * delta_dist_y;
//     }
//     else // Moving up
//     {
//         step_y = 1;
//         horizontal_y_dist = ((map_y + 1) * cell_size - player_y) * delta_dist_y;
//     }

//     bool hit = false;
//     int side = 0;

//     // Perform DDA
//     while (!hit)
//     {
//         // Jump to next map square, either in x-direction or in y-direction
//         if (vertical_x_dist < horizontal_y_dist)
//         {
//             vertical_x_dist += delta_dist_x;
//             map_x += step_x;
//             side = 0; // Vertical hit
//         }
//         else
//         {
//             horizontal_y_dist += delta_dist_y;
//             map_y += step_y;
//             side = 1; // Horizontal hit
//         }
        
//         // Check if ray has hit a wall
//         if (map[map_y][map_x] > 0)
//         {
//             hit = true;
//         }
//     }

//     // Calculate exact hit position for rendering
//     float perp_wall_dist;
//     if (side == 0)
//     {
//         perp_wall_dist = (map_x * cell_size - player_x + (1 - step_x) * cell_size / 2) / ray.x;
//     }
//     else
//     {
//         perp_wall_dist = (map_y * cell_size - player_y + (1 - step_y) * cell_size / 2) / ray.y;
//     }

//     float hit_x = player_x + ray.x * perp_wall_dist;
//     float hit_y = player_y + ray.y * perp_wall_dist;

//     // Render the ray using SDL
//     SDL_RenderDrawLineF(RaygineRenderer::GetRenderer(), player_x, player_y, hit_x, hit_y);
// }

void DrawRay(float player_x, float player_y, float dx, float dy, float player_angle, Player* player)
{
    int map_x = (int)player->pos.x / cell_size;
    int map_y = (int) player->pos.y / cell_size;

    Vec2<float> ray_start = {
        player->pos.x / cell_size, 
        player->pos.y / cell_size
    };

    Vec2<float> ray_dir = {
        player->dir.x,
        player->dir.y
    };

    Vec2<float> ray_unit_step_size = {
        sqrt(1 + ((ray_dir.y / ray_dir.x) * (ray_dir.y / ray_dir.x))),
        sqrt(1 + ((ray_dir.x / ray_dir.y) * (ray_dir.x / ray_dir.y)))
    };

    Vec2<float> ray_length = { 0.0f, 0.0f };

    Vec2<int> step = { 0, 0 };

    if (ray_dir.x < 0)
    {
        step.x = -1;
        ray_length.x = (ray_start.x - float(map_x)) * ray_unit_step_size.x;
    }
    else
    {
        step.x = 1;
        ray_length.x = (float(map_x + 1) - ray_start.x) * ray_unit_step_size.x;
    }
    if (ray_dir.y < 0)
    {
        step.y = -1;
        ray_length.y = (ray_start.y - float(map_y)) * ray_unit_step_size.y;
    }
    else
    {
        step.y = 1;
        ray_length.y = (float(map_y + 1) - ray_start.y) * ray_unit_step_size.y;
    }

    bool wall_hit = false;
    float distance = 0.0f;
    float max_distance = 100000.0f;
    while (!wall_hit && distance < max_distance)
    {
        if (ray_length.x < ray_length.y)
        {
            map_x += step.x;
            distance = ray_length.x;
            ray_length.x += ray_unit_step_size.x;
        }
        else
        {
            map_y += step.y;
            distance = ray_length.y;
            ray_length.y += ray_unit_step_size.y;
        }
        if (map_x >= 0 && map_x < map[0].size() && map_y >= 0 && map_y < map.size())
        {
            if (map[map_y][map_x] == 1)
            {
                wall_hit = true;
            }
        }
    }

    Vec2<float> intersection = { 0.0f, 0.0f };
    if (wall_hit)
    {
        intersection.x = (ray_start.x * cell_size) + (ray_dir.x * cell_size) * distance;
        intersection.y = (ray_start.y * cell_size) + (ray_dir.y * cell_size) * distance;
        SDL_SetRenderDrawColor(RaygineRenderer::GetRenderer(), 0, 255, 0, 255);
        SDL_RenderDrawLineF(RaygineRenderer::GetRenderer(), player->pos.x, player->pos.y, intersection.x, intersection.y);
    }
}





int main()
{
    Player player = {
        { 145.0f, 85.0f },
        { 1.0f, 0.0f }
    };
    // float player_pos_x = 145.0f;
    // float player_pos_y = 85.0f;
    // float player_delta_x = 1.0f;
    // float player_delta_y = 0.0f;
    float player_angle = 0.0f;

    InitSDL();
    // RaygineRenderer::InitWindow(1200, 600); 
    RaygineRenderer::InitWindow(window_width, window_height); // TODO: FIX ME!!!
    RaygineRenderer::CreateRenderer();
    draw_map();
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
                        player.pos.x += player.dir.x * 5;
                        player.pos.y += player.dir.y * 5;
                    }
                    break;
                    case SDLK_s: // Move backward
                    {
                        player.pos.x -= player.dir.x * 5;
                        player.pos.y -= player.dir.y * 5;
                    }
                    break;
                    case SDLK_a: // Rotate left (counter-clockwise)
                    {
                        player_angle += 2; 
                        if (player_angle > 360) 
                        {
                            player_angle -= 360;
                        }
                        player.dir.x = cos(degree_to_rad(player_angle));
                        player.dir.y= -sin(degree_to_rad(player_angle));
                    }
                    break;
                    case SDLK_d:  // Rotate right (clockwise)
                    {
                        player_angle -= 2; 
                        if (player_angle < 0) 
                        {
                            player_angle += 360;
                        }
                        player.dir.x = cos(degree_to_rad(player_angle));
                        player.dir.y = -sin(degree_to_rad(player_angle));
                    }
                    break;
                }
            }
        }
        // update
        // todo.
        // render
        RaygineRenderer::SetDrawColor(0, 0, 0, 255);
        RaygineRenderer::ClearRenderer();
        draw_map();
#ifdef DEBUG
        std::cout << "x: " << player.pos.x << ", y: " << player.pos.y << ", angle: " << player_angle << "\n";
        std::cout << "delta_x: " << player.dir.x << ", delta_y: " << player.dir.y << std::endl;
#endif
        DrawRay(player.pos.x, player.pos.y, player.dir.x, player.dir.y, player_angle, &player);
        draw_player(player.pos.x, player.pos.y, player.dir.x, player.dir.y);
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