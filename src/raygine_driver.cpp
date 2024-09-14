#include <algorithm> // For std::clamp
#include <cmath>
#include <iostream>
#include <limits>
#include <SDL2/SDL.h>
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

enum HitType {
    VERTICAL,
    HORIZONTAL, 
    NONE
};

typedef struct {
    HitType type;
    float distance;
} HitInfo;

HitInfo DrawRay(Vec2<float> ray_dir, Player* player)
{
    // Find out what tile we are in.
    int map_x = (int)player->pos.x / cell_size;
    int map_y = (int) player->pos.y / cell_size;

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
            if (map[map_y][map_x] == 1)
            {
                wall_hit = true;
            }
        }
    }

    // Draw the intersection point.
    // We need to convert everything from map space to "pixel" space.
    Vec2<float> intersection = { 0.0f, 0.0f };
    if (wall_hit)
    {
        intersection.x = (ray_start.x * cell_size) + (ray_dir.x * cell_size) * distance;
        intersection.y = (ray_start.y * cell_size) + (ray_dir.y * cell_size) * distance;
        RaygineRenderer::SetDrawColor(0, 255, 0, 255);
        RaygineRenderer::RenderDrawLineF(player->pos.x, player->pos.y, intersection.x, intersection.y);
    }
    return { hit_type, distance };
}

void DrawRays(float player_x, float player_y, float player_angle, Player* player, int num_rays, float fov)
{
    float angle_step = fov / float(num_rays - 1);
    float projection_plane_distance = (window_width / 2.0f) / (tan(fov / 2.0f));



    for (int i = 0; i < num_rays; i++)
    {
        // Calculate the angle for this ray.
        float ray_angle_offset = -fov / 2.0f + i * angle_step;

        // Calculate the new direction vector for this ray.
        Vec2<float> ray_dir = {
            player->dir.x * cos(degree_to_rad(ray_angle_offset)) - player->dir.y * sin(degree_to_rad(ray_angle_offset)),
            player->dir.x * sin(degree_to_rad(ray_angle_offset)) + player->dir.y * cos(degree_to_rad(ray_angle_offset))
        };

        // Cast the ray using the calculated direction.
        HitInfo hit_info = DrawRay(ray_dir, player);

        // Correct fisheye.
        hit_info.distance *= cos(degree_to_rad(ray_angle_offset));

        int wall_height = (int)(projection_plane_distance / hit_info.distance);

        int wall_start = (window_height / 2) - (wall_height / 2);
        int wall_end = (window_height / 2) + (wall_height / 2);

        // Set color based on hit type
        if (hit_info.type == HitType::VERTICAL)
        {
            RaygineRenderer::SetDrawColor(150, 0, 0, 255);
        }
        else
        {
            RaygineRenderer::SetDrawColor(255, 0, 0, 255);
        }

        // Draw filled rectangle for the ray, scaled to half the window width
        SDL_Rect wall_rect = { 
            (window_width / 2) + i * (window_width / (2 * num_rays)), 
            wall_start, 
            (window_width / (2 * num_rays)),
            wall_height 
        };
        RaygineRenderer::RenderFillRect(&wall_rect);

        // Draw ceiling.
        SDL_Rect ceil_rect = {
            wall_rect.x, 
            0,
            wall_rect.w,
            wall_start
        };
        RaygineRenderer::SetDrawColor(100, 100, 100, 255);
        RaygineRenderer::RenderFillRect(&ceil_rect);

        // Draw floor.
        SDL_Rect floor_rect = {
            wall_rect.x,
            wall_rect.y + wall_rect.h,
            wall_rect.w,
            window_height - (wall_rect.y + wall_rect.h)
        };
        RaygineRenderer::SetDrawColor(20, 150, 20, 255);
        RaygineRenderer::RenderFillRect(&floor_rect);
    }
}





int main()
{
    // TODO: Make cell_size configurable...
    RaygineRenderer::SetCellSize(cell_size);
    // TODO: Temp for the moment, will need to remove in the future. Load map.
    Map map = MapLoader::LoadLevel("level_1");
    Player player = {
        { 145.0f, 85.0f },
        { 1.0f, 0.0f }
    };

    float player_angle = 0.0f;

    InitSDL();
    RaygineRenderer::InitWindow(window_width, window_height);
    RaygineRenderer::CreateRenderer();
    // draw_map();
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
        DrawRays(player.pos.x, player.pos.y, player_angle, &player, 200, 65);
        RaygineRenderer::DrawPlayer(player.pos, player.dir);
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