#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include "../include/raygine_renderer.hpp"
#include <utility> // For std::pair.
#include <limits>
#include <algorithm> // For std::clamp

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

    // Calculate the end point of the direction line
    float line_length = 20.0f; // Length of the direction line
    float end_x = player_x + player_dir_x * line_length;
    float end_y = player_y + player_dir_y * line_length;

    // Draw the direction line
    SDL_SetRenderDrawColor(RaygineRenderer::GetRenderer(), 255, 0, 0, 255); // Green color for the direction line
    SDL_RenderDrawLineF(RaygineRenderer::GetRenderer(), player_x, player_y, end_x, end_y);
}

void DrawRay(float player_x, float player_y, float dx, float dy, float player_angle)
{
    const float FOV = 60.0f; // Define your field of view
    const int num_rays = 240; // Increase the number of rays for smoother render
    const float ray_increment = FOV / num_rays; // Increment per ray
    float ray_angle = player_angle - (FOV / 2.0f); // Start angle adjusted for half the FOV

    for (int i = 0; i < num_rays; i++)
    {
        bool hit_wall = false;
        float ray_x = 0.0f, ray_y = 0.0f, x_offset = 0.0f, y_offset = 0.0f;
        float horizontal_ray_x = 0.0f, horizontal_ray_y = 0.0f, horizontal_distance = std::numeric_limits<float>::max();
        float shortest_distance = 0.0f;

        // Horizontal grid check
        if (degree_to_rad(ray_angle) == 0 || degree_to_rad(ray_angle) == PI)
        {
            ray_x = player_x; 
            ray_y = player_y;
        }
        else if (degree_to_rad(ray_angle) > PI) // Player is looking down
        {
            ray_y = ((int)(player_y / cell_size)) * cell_size + cell_size; // Round up to nearest cell
            ray_x = player_x + (player_y - ray_y) / tan(degree_to_rad(ray_angle));
            y_offset = cell_size;
            x_offset = -cell_size * (1 / tan(degree_to_rad(ray_angle)));
        }
        else if (degree_to_rad(ray_angle) < PI) // Player is looking up
        {
            ray_y = (((int)(player_y / cell_size)) * cell_size) - 0.001; // Round down to nearest cell
            ray_x = player_x + (player_y - ray_y) / tan(degree_to_rad(ray_angle));
            y_offset = -cell_size;
            x_offset = cell_size * (1 / tan(degree_to_rad(ray_angle)));
        } 
        int counter = 0;
        while (!hit_wall && counter != 10)
        {
            std::pair<int, int> int_ray = ray_to_map_coordinates(ray_x, ray_y, cell_size);
            int int_ray_x = int_ray.first;
            int int_ray_y = int_ray.second;
            if ((int_ray_y < map.size() && int_ray_x < map[int_ray_y].size()) && map[int_ray_y][int_ray_x] == 1)
            {
                counter = 10;
                horizontal_ray_x = ray_x;
                horizontal_ray_y = ray_y;
                horizontal_distance = distance_formula(ray_x, ray_y, player_x, player_y);
            }
            else
            {
                ray_x += x_offset;
                ray_y += y_offset;
                counter++;
            }
        }

        float vertical_ray_x = 0.0f, vertical_ray_y = 0.0f, vertical_distance = std::numeric_limits<float>::max();
        // Vertical line check
        if (degree_to_rad(ray_angle) == (PI / 2) || degree_to_rad(ray_angle) == (3 * PI / 2))
        {
            ray_x = player_x; 
            ray_y = player_y;
        }
        else if (degree_to_rad(ray_angle) < (PI / 2) || degree_to_rad(ray_angle) > (3 * PI / 2)) // Player is looking right
        {
            ray_x = ((int)(player_x / cell_size)) * cell_size + cell_size; // Round up to nearest cell
            ray_y = player_y + (player_x - ray_x) * tan(degree_to_rad(ray_angle));
            y_offset = -cell_size * tan(degree_to_rad(ray_angle));
            x_offset = cell_size;
        }
        else if (degree_to_rad(ray_angle) > (PI / 2) && degree_to_rad(ray_angle) < (3 * PI / 2)) // Player is looking left
        {
            ray_x = (((int)(player_x / cell_size)) * cell_size) - 0.001; // Round down to nearest cell
            ray_y = player_y + (player_x - ray_x) * tan(degree_to_rad(ray_angle));
            y_offset = cell_size * tan(degree_to_rad(ray_angle));
            x_offset = -cell_size;
        } 
        SDL_SetRenderDrawColor(RaygineRenderer::GetRenderer(), 0, 255, 0, 255);
        counter = 0;
        while (counter != 10)
        {
            std::pair<int, int> int_ray = ray_to_map_coordinates(ray_x, ray_y, cell_size);
            int int_ray_x = int_ray.first;
            int int_ray_y = int_ray.second;
            if ((int_ray_y < map.size() && int_ray_x < map[int_ray_y].size()) && map[int_ray_y][int_ray_x] == 1)
            {
                counter = 10;
                vertical_ray_x = ray_x;
                vertical_ray_y = ray_y;
                vertical_distance = distance_formula(ray_x, ray_y, player_x, player_y);
            }
            else
            {
                ray_x += x_offset;
                ray_y += y_offset;
                counter++;
            }
        }
        Uint8 red = 255;
        if (vertical_distance < horizontal_distance)
        {
            ray_x = vertical_ray_x;
            ray_y = vertical_ray_y;
            shortest_distance = vertical_distance;
            // Uint8 red = 255;
        }
        else
        {
            ray_x = horizontal_ray_x;
            ray_y = horizontal_ray_y;
            shortest_distance = horizontal_distance;
            red = 150;
        }

        SDL_RenderDrawLineF(RaygineRenderer::GetRenderer(), player_x, player_y, ray_x, ray_y);

        // Draw 3D projection
        float correction_angle = player_angle - ray_angle;
        correction_angle = MakeInBounds(correction_angle);
        shortest_distance = shortest_distance * cos(degree_to_rad(correction_angle));

        float line_height = (cell_size * window_height) / shortest_distance;
        line_height = std::clamp(line_height, 0.0f, static_cast<float>(window_height)); // Clamp to window height
        float line_offset = (window_height / 2) - (line_height / 2);

        // Calculate x position for the slice
        float screen_x = i * (window_width / static_cast<float>(num_rays)) + window_width / 2;

        // Draw the wall slice as a filled rectangle
        SDL_Rect wall_rect;
        wall_rect.x = static_cast<int>(screen_x);
        wall_rect.y = static_cast<int>(line_offset);
        wall_rect.w = std::max(1, window_width / num_rays) + 1; // Width of the rectangle (slice width), ensure at least 1 pixel wide
        wall_rect.h = static_cast<int>(line_height); // Height of the rectangle

        // Calculate shading factor based on distance
        float max_distance = 800.0f; // Adjust based on your map and scene size
        float shading_factor = std::clamp(1.0f - (shortest_distance / max_distance), 0.3f, 1.0f); // Clamped to ensure it doesn't get too dark

        // Calculate the shaded color (darker when further away)
        // Uint8 red = static_cast<Uint8>(255 * shading_factor);
        Uint8 green = static_cast<Uint8>(0 * shading_factor);  // Adjust green if needed
        Uint8 blue = static_cast<Uint8>(0 * shading_factor);   // Adjust blue if needed

        SDL_SetRenderDrawColor(RaygineRenderer::GetRenderer(), red, green, blue, 255); // Shaded wall color
        SDL_RenderFillRect(RaygineRenderer::GetRenderer(), &wall_rect);

        ray_angle += ray_increment; // Move to the next ray angle
        ray_angle = MakeInBounds(ray_angle); // Keep the angle in bounds [0, 360)
    }
}




int main()
{
    float player_pos_x = 145.0f;
    float player_pos_y = 85.0f;
    float player_delta_x = 0.0f;
    float player_delta_y = 0.0f;
    float player_angle = 0.0f;

    InitSDL();
    RaygineRenderer::InitWindow(1200, 600); 
    // RaygineRenderer::InitWindow(window_width, window_height); // TODO: FIX ME!!!
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
                        player_pos_x += player_delta_x * 5;
                        player_pos_y += player_delta_y * 5;
                    }
                    break;
                    case SDLK_s: // Move backward
                    {
                        player_pos_x -= player_delta_x * 5;
                        player_pos_y -= player_delta_y * 5;
                    }
                    break;
                    case SDLK_d: // Rotate left (counter-clockwise)
                    {
                        player_angle += 2; // Adjust rotation speed as needed
                        if (player_angle > 360) 
                        {
                            player_angle -= 360;
                        }
                        player_delta_x = cos(degree_to_rad(player_angle));
                        player_delta_y = -sin(degree_to_rad(player_angle));
                    }
                    break;
                    case SDLK_a:  // Rotate right (clockwise)
                    {
                        player_angle -= 2; // Adjust rotation speed as needed
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
        // todo.
        // render
        RaygineRenderer::SetDrawColor(0, 0, 0, 255);
        RaygineRenderer::ClearRenderer();
        draw_map();
        // draw_player(player_pos_x, player_pos_y, player_delta_x, player_delta_y);
        #ifdef DEBUG
        std::cout << "x: " << player_pos_x << ", y: " << player_pos_y << ", angle: " << player_angle << "\n";
        std::cout << "delta_x: " << player_delta_x << ", delta_y: " << player_delta_y << std::endl;
        #endif
        DrawRay(player_pos_x, player_pos_y, player_delta_x, player_delta_y, player_angle);
        draw_player(player_pos_x, player_pos_y, player_delta_x, player_delta_y);
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