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

typedef struct {
    float x;
    float y;
    size_t id; 
    int texture_index;
    float scale;
} Sprite;

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

void draw_enemies_on_overhead(std::vector<Sprite>& e)
{
    for (auto& enemy : e)
    {
        RaygineRenderer::SetDrawColor(0, 0, 255, 255);
        SDL_FRect r = {enemy.x, enemy.y, 5, 5};
        RaygineRenderer::RenderFillRectF(&r);
    }
}

// void DrawSprites(Player* player, std::vector<Sprite> sprites, SDL_Texture** sprite_textures, int texWidth, int texHeight, float fov, const int height_adjustment)
// {
//     float projection_plane_distance = (window_width / 2.0f) / tan(degree_to_rad(fov / 2.0f)) - height_adjustment;
//     const float some_scaling_factor = 10.0f; // Adjust this value to scale sprite sizes appropriately

//     for (const auto& sprite : sprites)
//     {
//         // Calculate the distance from the player to the sprite
//         float spriteX = sprite.x - player->pos.x;
//         float spriteY = sprite.y - player->pos.y;
//         float distance = sqrt(spriteX * spriteX + spriteY * spriteY);

//         // Calculate the angle between the player's view direction and the direction to the sprite
//         float sprite_angle = atan2(spriteY, spriteX) - atan2(player->dir.y, player->dir.x);

//         // Normalize the angle to the range [-PI, PI]
//         if (sprite_angle < -M_PI) sprite_angle += 2 * M_PI;
//         if (sprite_angle > M_PI) sprite_angle -= 2 * M_PI;

//         // Check if the sprite is within the player's field of view
//         if (fabs(sprite_angle) < (fov / 2.0f))
//         {
//             // Calculate the height of the sprite on the screen
//             int sprite_height = static_cast<int>((window_height / distance) * some_scaling_factor);

//             // Calculate the vertical position of the sprite on the screen
//             int sprite_top = (window_height / 2) - (sprite_height / 2);
//             int sprite_bottom = sprite_top + sprite_height;

//             // Calculate the horizontal position of the sprite on the screen
//             int sprite_screen_x = (int)((window_width / 2) * (1 + tan(sprite_angle) / tan(degree_to_rad(fov / 2.0f))));

//             // Define the source rectangle from the sprite texture
//             SDL_Rect src_rect = { 0, 0, texWidth, texHeight };
//             SDL_Rect dest_rect = { sprite_screen_x - (sprite_height / 2), sprite_top, sprite_height, sprite_height };

//             // Render the sprite
//             SDL_RenderCopy(RaygineRenderer::GetRenderer(), sprite_textures[sprite.texture_index], &src_rect, &dest_rect);
//         }
//     }
// }


// void draw_sprite(Sprite &sprite, SDL_Renderer* renderer, Player &player, SDL_Texture* sprite_texture, int screen_width, int screen_height, float fov) 
// {
//     int sprite_texture_height, sprite_texture_width;
//     SDL_QueryTexture(sprite_texture, nullptr, nullptr, &sprite_texture_width, &sprite_texture_height);
    
//     // Calculate the direction from the player to the sprite
//     float sprite_dir = atan2(sprite.y - player.pos.y, sprite.x - player.pos.x);

//     // Calculate the player's direction angle
//     float player_dir = atan2(player.dir.y, player.dir.x);

//     // Normalize the direction relative to the player’s view
//     float relative_dir = sprite_dir - player_dir;
//     while (relative_dir > M_PI) relative_dir -= 2 * M_PI;
//     while (relative_dir < -M_PI) relative_dir += 2 * M_PI;

//     // Check if the sprite is within the player's field of view
//     if (fabs(relative_dir) < degree_to_rad(fov / 2.0f)) 
//     {
//         // Calculate the distance from the player to the sprite
//         float sprite_dist = std::sqrt(std::pow(player.pos.x - sprite.x, 2) + std::pow(player.pos.y - sprite.y, 2));

//         // Calculate the size of the sprite on the screen
//         // int sprite_screen_size = static_cast<int>(screen_height / sprite_dist); // Adjust this to scale as needed
//         int sprite_screen_size = 200;

//         // Calculate the horizontal offset where the sprite should be drawn
//         int h_offset = (relative_dir) * (screen_width / 2) / (fov) + (screen_width / 2) / 2 - sprite_screen_size / 2;

//         // Calculate the vertical offset where the sprite should be drawn
//         int v_offset = screen_height / 2 - sprite_screen_size / 2;

//         // Define the source rectangle from the sprite texture
//         SDL_Rect src_rect = {0, 0, sprite_texture_width, sprite_texture_height}; // Assumes the entire texture is used

//         // Define the destination rectangle where the sprite will be drawn on the screen
//         SDL_Rect dest_rect = {
//             h_offset + screen_width / 2, // horizontal position
//             v_offset, // vertical position
//             sprite_screen_size, // width of the sprite on screen
//             sprite_screen_size  // height of the sprite on screen
//         };

//         // Render the sprite
//         SDL_RenderCopy(renderer, sprite_texture, &src_rect, &dest_rect);
//     }
// }

Vec2<float> normalize(const Vec2<float>& v) {
    float length = sqrt(v.x * v.x + v.y * v.y);
    if (length != 0) {
        return { v.x / length, v.y / length };
    } else {
        return { 0.0f, 0.0f }; // Handle zero-length vector
    }
}


// void draw_sprite(Sprite &sprite, SDL_Renderer* renderer, Player &player, SDL_Texture* sprite_texture, int screen_width, int screen_height, float fov) 
// {
//     int sprite_texture_width, sprite_texture_height;
//     SDL_QueryTexture(sprite_texture, nullptr, nullptr, &sprite_texture_width, &sprite_texture_height);
    
//     // Ensure the player's direction vector is normalized
//     player.dir = normalize(player.dir);
    
//     // Calculate the direction from the player to the sprite
//     float dx = sprite.x - player.pos.x;
//     float dy = sprite.y - player.pos.y;
//     float sprite_dir = atan2(dy, dx);

//     // Calculate the player's direction angle
//     float player_dir = atan2(player.dir.y, player.dir.x);

//     // Calculate the relative angle between the sprite and the player's viewing direction
//     float relative_dir = sprite_dir - player_dir;

//     // Normalize the angle to the range [-π, π]
//     if (relative_dir > M_PI) relative_dir -= 2 * M_PI;
//     if (relative_dir < -M_PI) relative_dir += 2 * M_PI;

//     // Convert FOV to radians if it's in degrees
//     float fov_rad = degree_to_rad(fov); // Ensure this function converts degrees to radians

//     // Check if the sprite is within the player's field of view
//     if (fabs(relative_dir) < (fov_rad / 2.0f)) 
//     {
//         // Calculate the distance from the player to the sprite
//         float sprite_dist = sqrt(dx * dx + dy * dy);

//         // Perform z-buffering or depth sorting here if necessary

//         // Calculate the size of the sprite on the screen (adjust as needed)
//         int sprite_screen_size = static_cast<int>((screen_height / sprite_dist) * sprite.scale);

//         // Calculate the horizontal position where the sprite should be drawn
//         int screen_x_center = screen_width / 2;
//         float angle_to_pixel = relative_dir / (fov_rad / 2.0f);
//         int h_offset = static_cast<int>(angle_to_pixel * screen_x_center) - (sprite_screen_size / 2);

//         // Calculate the vertical position where the sprite should be drawn
//         int v_offset = (screen_height / 2) - (sprite_screen_size / 2);

//         // Define the source rectangle from the sprite texture
//         SDL_Rect src_rect = {0, 0, sprite_texture_width, sprite_texture_height};

//         // Define the destination rectangle where the sprite will be drawn on the screen
//         // SDL_Rect dest_rect = {
//         //     h_offset + screen_x_center + (window_width / 2), // horizontal position
//         //     v_offset,                   // vertical position
//         //     sprite_screen_size,         // width of the sprite on screen
//         //     sprite_screen_size          // height of the sprite on screen
//         // };
//         SDL_FRect dest_rect = {
//             sprite.x + (window_width/2), // Adjust x position to center the sprite
//             sprite.y + 200, // Adjust y position to move the origin to the bottom
//             200,
//             200
//         };
//         dest_rect.x -= dest_rect.w / 2;
//         dest_rect.y -= dest_rect.h;

//         // Render the sprite
//         // SDL_RenderCopy(renderer, sprite_texture, &src_rect, &dest_rect);
//         SDL_RenderCopyExF(renderer, sprite_texture, &src_rect, &dest_rect, 0, nullptr, SDL_FLIP_NONE);
        
//     }
// }

// void draw_sprite(Sprite &sprite, SDL_Renderer* renderer, Player &player, SDL_Texture* sprite_texture, int screen_width, int screen_height, float fov)
// {
//     int texWidth, texHeight;
//     SDL_QueryTexture(sprite_texture, nullptr, nullptr, &texWidth, &texHeight);

//     // Calculate the angle between the sprite and the player
//     float dx = sprite.x - player.pos.x;
//     float dy = sprite.y - player.pos.y;
//     float sprite_angle = atan2(dy, dx) - atan2(player.dir.y, player.dir.x);

//     // Normalize the angle to the range [-PI, PI]
//     if (sprite_angle > M_PI) sprite_angle -= 2 * M_PI;
//     if (sprite_angle < -M_PI) sprite_angle += 2 * M_PI;

//     // Convert FOV to radians
//     float fov_rad = degree_to_rad(fov);

//     // Check if the sprite is within the player's FOV
//     float half_fov = fov_rad / 2.0f;
//     if (fabs(sprite_angle) > half_fov)
//         return; // Sprite is not within FOV

//     // Calculate distance to the sprite
//     float sprite_dist = sqrt(dx * dx + dy * dy);

//     // Correct for fisheye effect
//     float perp_dist = sprite_dist * cos(sprite_angle);

//     // Calculate sprite height and width on screen
//     float projection_plane_distance = (screen_width / 2.0f) / tan(half_fov);
//     int sprite_screen_height = (int)((cell_size / perp_dist) * projection_plane_distance);
//     int sprite_screen_width = sprite_screen_height; // Assuming square sprites

//     // Calculate sprite's top and bottom Y positions on screen
//     int sprite_screen_top_y = (screen_height / 2) - (sprite_screen_height / 2);
//     int sprite_screen_bottom_y = (screen_height / 2) + (sprite_screen_height / 2);

//     // Calculate the horizontal position of the sprite on screen
//     float sprite_screen_x = (screen_width / 2) + (tan(sprite_angle) * projection_plane_distance) - (sprite_screen_width / 2);

//     // Determine the start and end X positions on screen
//     int sprite_screen_x_start = (int)sprite_screen_x;
//     int sprite_screen_x_end = sprite_screen_x_start + sprite_screen_width;

//     // Clip sprite horizontally
//     if (sprite_screen_x_start < 0) sprite_screen_x_start = 0;
//     if (sprite_screen_x_end > screen_width) sprite_screen_x_end = screen_width;

//     // Render the sprite column by column
//     for (int x = sprite_screen_x_start; x < sprite_screen_x_end; x++)
//     {
//         // Calculate the corresponding texture X coordinate
//         int tex_x = (int)((x - sprite_screen_x) * texWidth / sprite_screen_width);

//         // Check depth buffer for occlusion
//         // if (perp_dist < depth_buffer[x])
//         {
//             // Render the sprite column by column
//             SDL_Rect src_rect = { tex_x, 0, 1, texHeight };
//             SDL_Rect dest_rect = { x, sprite_screen_top_y, 1, sprite_screen_height };
//             SDL_RenderCopy(renderer, sprite_texture, &src_rect, &dest_rect);
//         }
//     }
// }


// void draw_sprite(Sprite &sprite, SDL_Renderer* renderer, Player &player, SDL_Texture* sprite_texture, int screen_width, int screen_height, float fov)
// {
//     int texWidth, texHeight;
//     SDL_QueryTexture(sprite_texture, nullptr, nullptr, &texWidth, &texHeight);
//     float sprite_aspect_ratio = (float)texWidth / (float)texHeight;


//     // Calculate the angle between the sprite and the player
//     float dx = sprite.x - player.pos.x;
//     float dy = sprite.y - player.pos.y;
//     float sprite_angle = atan2(dy, dx) - atan2(player.dir.y, player.dir.x);

//     // Normalize the angle to the range [-PI, PI]
//     if (sprite_angle > M_PI) sprite_angle -= 2 * M_PI;
//     if (sprite_angle < -M_PI) sprite_angle += 2 * M_PI;

//     // Convert FOV to radians
//     float fov_rad = degree_to_rad(fov);

//     // Check if the sprite is within the player's FOV
//     float half_fov = fov_rad / 2.0f;
//     if (fabs(sprite_angle) > half_fov)
//         return; // Sprite is not within FOV

//     // Calculate distance to the sprite
//     float sprite_dist = sqrt(dx * dx + dy * dy);

//     // Correct for fisheye effect
//     float perp_dist = sprite_dist * cos(sprite_angle);

//     // **Adjust for half-screen rendering**
//     int render_width = screen_width / 2;       // The width of the 3D rendering area
//     int render_x_offset = screen_width / 2;    // The x offset where rendering starts

//     // Calculate sprite height and width on screen
//     float projection_plane_distance = (render_width / 2.0f) / tan(half_fov);
//     int sprite_screen_height = (int)((cell_size / perp_dist) * projection_plane_distance);
//     int sprite_screen_width = sprite_screen_height; // Assuming square sprites

//     // Calculate sprite's top Y position on screen
//     int sprite_screen_top_y = (screen_height / 2) - (sprite_screen_height / 2);

//     // Calculate the horizontal position of the sprite on screen
//     float sprite_screen_x = render_x_offset + (render_width / 2) + (tan(sprite_angle) * projection_plane_distance) - (sprite_screen_width / 2);

//     // Determine the start and end X positions on screen
//     int sprite_screen_x_start = (int)sprite_screen_x;
//     int sprite_screen_x_end = sprite_screen_x_start + sprite_screen_width;

//     // Clip sprite horizontally to the rendering area
//     if (sprite_screen_x_start < render_x_offset) sprite_screen_x_start = render_x_offset;
//     if (sprite_screen_x_end > screen_width) sprite_screen_x_end = screen_width;

//     // Render the sprite column by column
//     for (int x = sprite_screen_x_start; x < sprite_screen_x_end; x++)
//     {
//         // Calculate the corresponding texture X coordinate
//         int tex_x = (int)((x - sprite_screen_x) * texWidth / sprite_screen_width);

//         // **Depth Buffer Check (optional, if you have a depth buffer)**
//         // int buffer_index = x - render_x_offset;
//         // if (perp_dist < depth_buffer[buffer_index])
//         {
//             // Render the sprite column by column
//             SDL_Rect src_rect = { tex_x, 0, 1, texHeight };
//             SDL_Rect dest_rect = { x, sprite_screen_top_y, 1, sprite_screen_height };
//             SDL_RenderCopy(renderer, sprite_texture, &src_rect, &dest_rect);
//         }
//     }
// }

void draw_sprite(Sprite &sprite, SDL_Renderer* renderer, Player &player, SDL_Texture* sprite_texture, int screen_width, int screen_height, float fov)
{
    int texWidth, texHeight;
    SDL_QueryTexture(sprite_texture, nullptr, nullptr, &texWidth, &texHeight);

    // Calculate the angle between the sprite and the player
    float dx = sprite.x - player.pos.x;
    float dy = sprite.y - player.pos.y;
    float sprite_angle = atan2(dy, dx) - atan2(player.dir.y, player.dir.x);

    // Normalize the angle to the range [-PI, PI]
    if (sprite_angle > M_PI) sprite_angle -= 2 * M_PI;
    if (sprite_angle < -M_PI) sprite_angle += 2 * M_PI;

    // Convert FOV to radians
    float fov_rad = degree_to_rad(fov);

    // Check if the sprite is within the player's FOV
    float half_fov = fov_rad / 2.0f;
    if (fabs(sprite_angle) > fov_rad)
        return; // Sprite is not within FOV

    // Calculate distance to the sprite
    float sprite_dist = sqrt(dx * dx + dy * dy);

    // Correct for fisheye effect
    float perp_dist = sprite_dist * cos(sprite_angle);

    // Adjust for half-screen rendering
    int render_width = screen_width / 2;       // The width of the 3D rendering area
    int render_x_offset = screen_width / 2;    // The x offset where rendering starts

    // Calculate projection plane distance
    float projection_plane_distance = (render_width / 2.0f) / tan(half_fov);

    // Calculate sprite height on screen
    int sprite_screen_height = (int)((cell_size / perp_dist) * projection_plane_distance);

    // Calculate sprite width on screen
    int sprite_screen_width = (int)((cell_size / perp_dist) * projection_plane_distance);

    // Calculate sprite's top Y position on screen
    int sprite_screen_top_y = (screen_height / 2) - (sprite_screen_height / 2);

    


    // Calculate the horizontal position of the sprite on screen
    float sprite_screen_x = render_x_offset + (render_width / 2) + (tan(sprite_angle) * projection_plane_distance) - (sprite_screen_width / 2);

    // Determine the start and end X positions on screen
    int sprite_screen_x_start = (int)sprite_screen_x;
    int sprite_screen_x_end = sprite_screen_x_start + sprite_screen_width;

    // Clip sprite horizontally to the rendering area
    if (sprite_screen_x_start < render_x_offset) sprite_screen_x_start = render_x_offset;
    if (sprite_screen_x_end > screen_width) sprite_screen_x_end = screen_width;

    // Render the sprite column by column
    for (int x = sprite_screen_x_start; x < sprite_screen_x_end; x++)
    {
        // Calculate the corresponding texture X coordinate
        int tex_x = (int)((x - sprite_screen_x) * texWidth / sprite_screen_width);

        // Depth buffer check (optional)
        // int buffer_index = x - render_x_offset;
        // if (perp_dist < depth_buffer[buffer_index])
        {
            // Render the sprite column by column
            SDL_Rect src_rect = { tex_x, 0, 1, texHeight };
            SDL_Rect dest_rect = { x, sprite_screen_top_y, 1, sprite_screen_height };
            // SDL_RenderDrawRect(renderer, &dest_rect);
            SDL_RenderCopy(renderer, sprite_texture, &src_rect, &dest_rect);
            
        }
    }
}






SDL_Texture* LoadTexture(const char * file_path)
{
    // Load the image into an SDL_Surface
    SDL_Surface* surface = IMG_Load(file_path);
    if (surface == nullptr)
    {
        std::cerr << "Failed to load image: " << IMG_GetError() << "\n";
        return nullptr;
    }

    // Convert the SDL_Surface to an SDL_Texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface(RaygineRenderer::GetRenderer(), surface);
    if (texture == nullptr)
    {
        std::cerr << "Failed to create texture: " << SDL_GetError() << "\n";
    }

    // Free the SDL_Surface as it is no longer needed
    SDL_FreeSurface(surface);

    return texture;
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

    // SDL_Texture* monster_texture = LoadTexture("../_levels/level_1/map/wall_textures/Chicken.png");
    SDL_Texture* monster_texture = LoadTexture("../_levels/level_1/map/wall_textures/monster.png");

    SDL_Texture* m_arr[1];
    m_arr[0] = monster_texture;
    int m_width, m_height;
    SDL_QueryTexture(monster_texture, nullptr, nullptr, &m_width, &m_height); 



    // Get texture dimensions
    int texWidth, texHeight;
    SDL_QueryTexture(wall_texture, nullptr, nullptr, &texWidth, &texHeight);

    // Load map
    Map map = MapLoader::LoadLevel("level_1");
    Player player = {
        { 145.0f, 85.0f },
        { 1.0f, 0.0f }
    };

    // init enemies
    std::vector<Sprite> enemies = {
        {100.0, 100.0, 0, 0, 5},
        {200.0, 250.0, 0, 0, 5},
        {250.0, 50.0, 0, 0, 5}
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
                    case SDLK_ESCAPE:
                    {
                        quit = true;
                    }
                    break;
                }
            }
        }
        // Update.
        // Render.
        RaygineRenderer::SetDrawColor(0, 0, 0, 255);
        RaygineRenderer::ClearRenderer();
        
#ifdef DEBUG
        std::cout << "x: " << player.pos.x << ", y: " << player.pos.y << ", angle: " << player_angle << "\n";
        std::cout << "delta_x: " << player.dir.x << ", delta_y: " << player.dir.y << std::endl;
#endif
        DrawRays(player.pos.x, player.pos.y, player_angle, &player, 200, 60, arr, arr, texWidth, texHeight);
        RaygineRenderer::DrawPlayer(player.pos, player.dir);
        draw_enemies_on_overhead(enemies);
        // draw_sprites(enemies, player);
        // DrawSprites(&player, enemies, m_arr, m_width, m_height, 60, 400);
        draw_sprite(enemies[0], RaygineRenderer::GetRenderer(), player, m_arr[0], window_width, window_height, 60);
        RaygineRenderer::DrawMap(map);
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
