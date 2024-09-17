#include <algorithm> // For std::clamp
#include <cmath>
#include <iostream>
#include <limits>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <utility> // For std::pair.
#include <vector>
#include "../include/map_loader.hpp"
#include "../include/raygine_renderer.hpp"
#include "../include/vec2.hpp"

#define DEBUG

const int window_width = 800;
const int window_height = 400;
const int cell_size = 50;
const float PI = 3.14159265359;

using namespace Raygine;

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

// std::vector<std::vector<int>> bottom = {
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 2, 2, 2, 2, 2, 2, 0},
//     {0, 2, 2, 2, 2, 2, 2, 0},
//     {0, 2, 2, 2, 2, 2, 2, 0},
//     {0, 2, 2, 2, 2, 2, 2, 0},
//     {0, 2, 2, 2, 2, 2, 2, 0},
//     {0, 2, 2, 2, 2, 2, 2, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0}
// };

std::vector<std::vector<int>> bottom = {
    {2, 2, 2, 2, 2, 2, 2, 2},
    {2, 2, 2, 2, 2, 2, 2, 2},
    {2, 2, 2, 2, 2, 2, 2, 2},
    {2, 2, 2, 2, 2, 2, 2, 2},
    {2, 2, 2, 2, 2, 2, 2, 2},
    {2, 2, 2, 2, 2, 2, 2, 2},
    {2, 2, 2, 2, 2, 2, 2, 2},
    {2, 2, 2, 2, 2, 2, 2, 2}
};

std::vector<std::vector<int>> top = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 3, 3, 3, 3, 3, 3, 0},
    {0, 3, 3, 3, 3, 3, 3, 0},
    {0, 3, 3, 3, 3, 3, 3, 0},
    {0, 3, 3, 3, 3, 3, 3, 0},
    {0, 3, 3, 3, 3, 3, 3, 0},
    {0, 3, 3, 3, 3, 3, 3, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
};

// std::vector<std::vector<int>> map = {
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
// };

// std::vector<std::vector<int>> bottom = {
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
//     {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
//     {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
//     {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
//     {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
//     {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
//     {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
//     {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
//     {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
//     {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
//     {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
//     {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
//     {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
//     {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
// };

// Need to have floor extend to the walls. If not, player can see the black
// -ness of the void. :|
// std::vector<std::vector<int>> bottom = {
//     {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
//     {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
//     {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
//     {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
//     {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
//     {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
//     {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
//     {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
//     {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
//     {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
//     {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
//     {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
//     {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
//     {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
//     {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
//     {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
// };

// std::vector<std::vector<int>> top = {
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0},
//     {0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0},
//     {0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0},
//     {0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0},
//     {0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0},
//     {0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0},
//     {0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0},
//     {0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0},
//     {0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0},
//     {0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0},
//     {0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0},
//     {0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0},
//     {0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0},
//     {0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
// };


void InitSDL();
float degree_to_rad(float in_degree)
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

typedef struct {
    Vec2<float> pos;
    Vec2<float> dir;
} Player;

enum HitType {
    VERTICAL,
    HORIZONTAL, 
    NONE
};

typedef struct {
    HitType type;
    float distance;
    Vec2<float> intersection;
    int surface_int;
} HitInfo;

HitInfo DrawRay(Vec2<float> ray_dir, Player* player)
{
    // Find out what tile we are in.
    int map_x = (int)player->pos.x / cell_size;
    int map_y = (int)player->pos.y / cell_size;

    // Convert player position to cell position.
    Vec2<float> ray_start = {
        player->pos.x / cell_size, 
        player->pos.y / cell_size
    };

    // Tells us how much to move along the ray that is cast.
    Vec2<float> ray_unit_step_size = {
        sqrt(1 + ((ray_dir.y / ray_dir.x) * (ray_dir.y / ray_dir.x))),
        sqrt(1 + ((ray_dir.x / ray_dir.y) * (ray_dir.x / ray_dir.y)))
    };

    Vec2<float> ray_length = { 0.0f, 0.0f };

    Vec2<int> step = { 0, 0 };

    // Calculate the initial step and step direction.
    if (ray_dir.x < 0)
    {
        step.x = -1;
        ray_length.x = (ray_start.x - static_cast<float>(map_x)) * ray_unit_step_size.x;
    }
    else
    {
        step.x = 1;
        ray_length.x = (static_cast<float>(map_x + 1) - ray_start.x) * ray_unit_step_size.x;
    }
    if (ray_dir.y < 0)
    {
        step.y = -1;
        ray_length.y = (ray_start.y - static_cast<float>(map_y)) * ray_unit_step_size.y;
    }
    else
    {
        step.y = 1;
        ray_length.y = (static_cast<float>(map_y + 1) - ray_start.y) * ray_unit_step_size.y;
    }

    // Perform DDA.
    bool wall_hit = false;
    float distance = 0.0f;
    float max_distance = 100000.0f;
    HitType hit_type;
    int surface_texture_type = 0;
    while (!wall_hit && distance < max_distance)
    {
        if (ray_length.x < ray_length.y)
        {
            map_x += step.x;
            distance = ray_length.x;
            ray_length.x += ray_unit_step_size.x;
            hit_type = HitType::HORIZONTAL;
        }
        else
        {
            map_y += step.y;
            distance = ray_length.y;
            ray_length.y += ray_unit_step_size.y;
            hit_type = HitType::VERTICAL;
        }
        if (map_x >= 0 && map_x < map[0].size() && map_y >= 0 && map_y < map.size())
        {
            if (map[map_y][map_x] > 0)
            {
                wall_hit = true;
                surface_texture_type = map[map_y][map_x];
            }
        }
    }

    Vec2<float> intersection = { 0.0f, 0.0f };
    if (wall_hit)
    {
        intersection.x = (ray_start.x * cell_size) + (ray_dir.x * cell_size) * distance;
        intersection.y = (ray_start.y * cell_size) + (ray_dir.y * cell_size) * distance;
        // Draw the intersection line (optional for debugging)
        RaygineRenderer::SetDrawColor(0, 255, 0, 255);
        RaygineRenderer::RenderDrawLineF(player->pos.x, player->pos.y, intersection.x, intersection.y);
    }
    return { hit_type, distance, intersection, surface_texture_type };
}


void DrawRays(float player_x, float player_y, float player_angle, Player* player, int num_rays, float fov, SDL_Texture** wall_texture, SDL_Texture** floor_texture, int texWidth, int texHeight)
{
    int height_adjustment = 400;
    float player_height = 0.5f;
    float angle_step = fov / float(num_rays - 1);
    // float projection_plane_distance = (window_width / 2.0f) / tan(degree_to_rad(fov / 2.0f));
    // This is with some odd scaling factor... This seems to effect the height. Subtracting a large value makes the height lower.
    // Wondering if I need player height? 
    float projection_plane_distance = (window_width / 2.0f) / tan(degree_to_rad(fov / 2.0f)) - height_adjustment;

    

    for (int i = 0; i < num_rays; i++)
    {
        float ray_angle_offset = -fov / 2.0f + i * angle_step;
        Vec2<float> ray_dir = {
            player->dir.x * cos(degree_to_rad(ray_angle_offset)) - player->dir.y * sin(degree_to_rad(ray_angle_offset)),
            player->dir.x * sin(degree_to_rad(ray_angle_offset)) + player->dir.y * cos(degree_to_rad(ray_angle_offset))
        };

        // Cast the ray and get hit information
        HitInfo hit_info = DrawRay(ray_dir, player);

        // Apply fisheye correction for wall
        hit_info.distance *= cos(degree_to_rad(ray_angle_offset));

        // Calculate wall height and vertical position
        int wall_height = (int)(projection_plane_distance / hit_info.distance);
        int wall_start = (window_height / 2) - (wall_height / 2);
        int wall_end = (window_height / 2) + (wall_height / 2);

        // Calculate texture X coordinate for wall
        int texX;
        if (hit_info.type == HitType::VERTICAL) {
            texX = (int)hit_info.intersection.x % cell_size;
        } else {
            texX = (int)hit_info.intersection.y % cell_size;
        }

        // Define the source rectangle from the wall texture
        SDL_Rect src_rect = { texX, 0, 1, texHeight };
        SDL_Rect dest_rect = { (window_width / 2) + i * (window_width / (2 * num_rays)), wall_start, (window_width / (2 * num_rays)), wall_height };
        SDL_Texture* tex = wall_texture[hit_info.surface_int - 1];

        // Render the wall slice
        SDL_RenderCopy(RaygineRenderer::GetRenderer(), tex, &src_rect, &dest_rect);

        // Draw ceiling texture line by line
        for (int y = 0; y < wall_start; y++)
        {
            // Adjust the row distance calculation for the ceiling
            // This scaling factor here is adjusting for the players height.
            float row_distance = (projection_plane_distance * cell_size * -player_height) / ((window_height / 2.0f - y) * cos(degree_to_rad(ray_angle_offset)));

            // Calculate the world position for this pixel
            float ceilingX = player->pos.x - row_distance * ray_dir.x;
            float ceilingY = player->pos.y - row_distance * ray_dir.y;

            // Calculate the texture coordinates
            int ceilingTexX = (int)(ceilingX * texWidth / cell_size) % texWidth;
            int ceilingTexY = (int)(ceilingY * texHeight / cell_size) % texHeight;

            // Ensure texture coordinates are positive
            if (ceilingTexX < 0) ceilingTexX += texWidth;
            if (ceilingTexY < 0) ceilingTexY += texHeight;

            // Select the appropriate ceiling texture based on the map
            int ceiling_cell_x = (int)(ceilingX / cell_size);
            int ceiling_cell_y = (int)(ceilingY / cell_size);

            // Render the ceiling pixel if within the map bounds
            if (ceiling_cell_x >= 0 && ceiling_cell_x < map[0].size() && ceiling_cell_y >= 0 && ceiling_cell_y < map.size())
            {
                int ceiling_texture_index = top[ceiling_cell_y][ceiling_cell_x] - 1;

                if (ceiling_texture_index >= 0)
                {
                    SDL_Rect ceiling_src_rect = { ceilingTexX, ceilingTexY, 1, 1 };
                    SDL_Rect ceiling_dest_rect = { 
                        (window_width / 2) + i * (window_width / (2 * num_rays)),
                        y, 
                        (window_width / (2 * num_rays)), 
                        1 
                    };
                    SDL_RenderCopy(RaygineRenderer::GetRenderer(), floor_texture[ceiling_texture_index], &ceiling_src_rect, &ceiling_dest_rect);
                }
            }
        }

        // // Draw floor texture line by line
        for (int y = wall_end; y < window_height; y++)
        {
            // Adjust the row distance calculation
            // Magic .48 is for the player's height.
            float row_distance = (projection_plane_distance * cell_size * player_height) / ((y - (window_height / 2.0f)) * cos(degree_to_rad(ray_angle_offset)));

            // Calculate the world position for this pixel
            float floorX = player->pos.x + row_distance * ray_dir.x;
            float floorY = player->pos.y + row_distance * ray_dir.y;

            // Calculate the texture coordinates
            int floorTexX = (int)(floorX * texWidth / cell_size) % texWidth;
            int floorTexY = (int)(floorY * texHeight / cell_size) % texHeight;

            // Select the appropriate floor texture based on the map
            int floor_cell_x = (int)(floorX / cell_size);
            int floor_cell_y = (int)(floorY / cell_size);

            // Render the floor pixel if within the map bounds
            if (floor_cell_x >= 0 && floor_cell_x < map[0].size() && floor_cell_y >= 0 && floor_cell_y < map.size())
            {
                int floor_texture_index = bottom[floor_cell_y][floor_cell_x] - 1;

                if (floor_texture_index >= 0)
                {
                    SDL_Rect floor_src_rect = { floorTexX, floorTexY, 1, 1 };
                    SDL_Rect floor_dest_rect = { 
                        (window_width / 2) + i * (window_width / (2 * num_rays)),
                        y, 
                        (window_width / (2 * num_rays)), 
                        1 
                    };
                    SDL_RenderCopy(RaygineRenderer::GetRenderer(), floor_texture[floor_texture_index], &floor_src_rect, &floor_dest_rect);
                }
            }
        }
    }
}




int main()
{
    // Initialize SDL and SDL_Image
    InitSDL();
    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        std::cerr << "Failed to initialize SDL_image: " << IMG_GetError() << "\n";
        exit(1);
    }

    // Initialize window and renderer
    RaygineRenderer::InitWindow(window_width, window_height);
    RaygineRenderer::CreateRenderer();

    // Set cell size
    RaygineRenderer::SetCellSize(cell_size);

    // Load the wall texture once
    SDL_Surface* g_wall_texture_surface = IMG_Load("../_levels/level_1/map/wall_textures/wp50.png");
    if (g_wall_texture_surface == nullptr)
    {
        std::cerr << "Failed to load wall texture: " << IMG_GetError() << "\n";
        exit(1);
    }
    // Convert surface to known pixel format
    SDL_Surface* optimized_wall_texture_surface = SDL_ConvertSurfaceFormat(g_wall_texture_surface, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(g_wall_texture_surface);
    g_wall_texture_surface = optimized_wall_texture_surface;
    // Create a texture from the surface
    SDL_Texture* wall_texture = SDL_CreateTextureFromSurface(RaygineRenderer::GetRenderer(), g_wall_texture_surface);
    SDL_FreeSurface(g_wall_texture_surface); // Free the surface as it's no longer needed

    if (!wall_texture)
    {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << "\n";
        exit(1);
    }

    ///////////
        // Load the wall texture once
    SDL_Surface* g_wall_texture_surface1 = IMG_Load("../_levels/level_1/map/wall_textures/floor50.png");
    if (g_wall_texture_surface == nullptr)
    {
        std::cerr << "Failed to load wall texture: " << IMG_GetError() << "\n";
        exit(1);
    }
    // Convert surface to known pixel format
    SDL_Surface* optimized_wall_texture_surface1 = SDL_ConvertSurfaceFormat(g_wall_texture_surface1, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(g_wall_texture_surface1);
    g_wall_texture_surface1 = optimized_wall_texture_surface1;
    // Create a texture from the surface
    SDL_Texture* wall_texture1 = SDL_CreateTextureFromSurface(RaygineRenderer::GetRenderer(), g_wall_texture_surface1);
    SDL_FreeSurface(g_wall_texture_surface1); // Free the surface as it's no longer needed

    if (!wall_texture)
    {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << "\n";
        exit(1);
    }
    ////////
        SDL_Surface* g_wall_texture_surface2 = IMG_Load("../_levels/level_1/map/wall_textures/ceil.png");
    if (g_wall_texture_surface == nullptr)
    {
        std::cerr << "Failed to load wall texture: " << IMG_GetError() << "\n";
        exit(1);
    }
    // Convert surface to known pixel format
    SDL_Surface* optimized_wall_texture_surface2 = SDL_ConvertSurfaceFormat(g_wall_texture_surface2, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(g_wall_texture_surface2);
    g_wall_texture_surface2 = optimized_wall_texture_surface2;
    // Create a texture from the surface
    SDL_Texture* wall_texture2 = SDL_CreateTextureFromSurface(RaygineRenderer::GetRenderer(), g_wall_texture_surface2);
    SDL_FreeSurface(g_wall_texture_surface2); // Free the surface as it's no longer needed

    if (!wall_texture)
    {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << "\n";
        exit(1);
    }

    SDL_Texture* arr[3];
    arr[0] = wall_texture;
    arr[1] = wall_texture1;
    arr[2] = wall_texture2;

    // Get texture dimensions
    int texWidth, texHeight;
    SDL_QueryTexture(wall_texture, nullptr, nullptr, &texWidth, &texHeight);

    // Load map
    Map map = MapLoader::LoadLevel("level_1");
    Player player = {
        { 145.0f, 85.0f },
        { 1.0f, 0.0f }
    };

    float player_angle = 0.0f;

    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        // Input
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
                        float neg_rotation_constant = -2;
                        player.dir = RotationMatrix2D<float>(player.dir, degree_to_rad(neg_rotation_constant));
                    }
                    break;
                    case SDLK_d:  // Rotate right (clockwise)
                    {
                        float rotation_constant = 2;
                        player.dir = RotationMatrix2D<float>(player.dir, degree_to_rad(rotation_constant));
                    }
                    break;
                }
            }
        }
        // Update.
        // Render.
        RaygineRenderer::SetDrawColor(0, 0, 0, 255);
        RaygineRenderer::ClearRenderer();
        RaygineRenderer::DrawMap(map);
#ifdef DEBUG
        std::cout << "x: " << player.pos.x << ", y: " << player.pos.y << ", angle: " << player_angle << "\n";
        std::cout << "delta_x: " << player.dir.x << ", delta_y: " << player.dir.y << std::endl;
#endif
        DrawRays(player.pos.x, player.pos.y, player_angle, &player, 200, 60, arr, arr, texWidth, texHeight);
        RaygineRenderer::DrawPlayer(player.pos, player.dir);
        SDL_RenderPresent(RaygineRenderer::GetRenderer());
    }

    // Cleanup
    SDL_DestroyTexture(wall_texture);
    RaygineRenderer::DestroyRender();
    RaygineRenderer::DestroyWindow();
    IMG_Quit();
    SDL_Quit();
    return 0;
}

void InitSDL()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cerr << "SDL failed to initialize: " << SDL_GetError() << "\n";
        exit(1);
    }
}
