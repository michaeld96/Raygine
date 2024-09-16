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
    {1, 0, 0, 0, 0, 0, 0, 2},
    {1, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}
};

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
                if (surface_texture_type == 2)
                {
                    printf("aaaa");
                }
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

// void DrawRays(float player_x, float player_y, float player_angle, Player* player, int num_rays, float fov, SDL_Texture* wall_texture, int texWidth, int texHeight)
// {
//     float angle_step = fov / float(num_rays - 1);
//     float projection_plane_distance = (window_width / 2.0f) / (tan(fov / 2.0f));

//     for (int i = 0; i < num_rays; i++)
//     {
//         // Calculate the angle for this ray.
//         float ray_angle_offset = -fov / 2.0f + i * angle_step;

//         // Calculate the new direction vector for this ray.
//         Vec2<float> ray_dir = {
//             player->dir.x * cos(degree_to_rad(ray_angle_offset)) - player->dir.y * sin(degree_to_rad(ray_angle_offset)),
//             player->dir.x * sin(degree_to_rad(ray_angle_offset)) + player->dir.y * cos(degree_to_rad(ray_angle_offset))
//         };

//         // Cast the ray using the calculated direction.
//         HitInfo hit_info = DrawRay(ray_dir, player);

//         // Correct fisheye.
//         hit_info.distance *= cos(degree_to_rad(ray_angle_offset));

//         int wall_height = (int)(projection_plane_distance / hit_info.distance);

//         int wall_start = (window_height / 2) - (wall_height / 2);
//         int wall_end = (window_height / 2) + (wall_height / 2);

//         // Calculate wallX
//         float wallX; // Exact position where wall was hit
//         if (hit_info.type == HitType::VERTICAL)
//         {
//             // If the wall is vertical, use the Y coordinate
//             wallX = hit_info.intersection.x / cell_size - floor(hit_info.intersection.x / cell_size);
//         }
//         else
//         {
//             // If the wall is horizontal, use the X coordinate
//             wallX = hit_info.intersection.y / cell_size - floor(hit_info.intersection.y / cell_size);
//         }

//         // Calculate texX
//         int texX = int(wallX * float(texWidth));
//         if (hit_info.type == HitType::VERTICAL)
//         {
//             texX = texWidth - texX - 1;
//         }

//         // Define the source rectangle from the texture
//         SDL_Rect src_rect = { texX, 0, 1, texHeight }; // 1 pixel wide slice

//         // Define the destination rectangle on the screen
//         int wall_x = (window_width / 2) + i * (window_width / (2 * num_rays));
//         int wall_width = (window_width / (2 * num_rays));

//         SDL_Rect dest_rect = { wall_x, wall_start, wall_width, wall_height };

//         // Render the wall slice
//         SDL_RenderCopy(RaygineRenderer::GetRenderer(), wall_texture, &src_rect, &dest_rect);

//         // Draw ceiling
//         SDL_Rect ceil_rect = {
//             dest_rect.x, 
//             0,
//             dest_rect.w,
//             wall_start
//         };
//         RaygineRenderer::SetDrawColor(100, 100, 100, 255);
//         RaygineRenderer::RenderFillRect(&ceil_rect);

//         // Draw floor
//         SDL_Rect floor_rect = {
//             dest_rect.x,
//             wall_end,
//             dest_rect.w,
//             window_height - wall_end
//         };
//         RaygineRenderer::SetDrawColor(20, 150, 20, 255);
//         RaygineRenderer::RenderFillRect(&floor_rect);
//     }
// }
void DrawRays(float player_x, float player_y, float player_angle, Player* player, int num_rays, float fov, SDL_Texture** wall_texture, int texWidth, int texHeight)
{
    float angle_step = fov / float(num_rays - 1);
    float projection_plane_distance = (window_width / 2.0f) / tan(fov / 2.0f);
    int ok = 0;
    for (int i = 0; i < num_rays; i++)
    {
        float ray_angle_offset = -fov / 2.0f + i * angle_step;
        Vec2<float> ray_dir = {
            player->dir.x * cos(degree_to_rad(ray_angle_offset)) - player->dir.y * sin(degree_to_rad(ray_angle_offset)),
            player->dir.x * sin(degree_to_rad(ray_angle_offset)) + player->dir.y * cos(degree_to_rad(ray_angle_offset))
        };

        // Cast the ray and get hit information
        HitInfo hit_info = DrawRay(ray_dir, player);

        // Apply fisheye correction
        hit_info.distance *= cos(degree_to_rad(ray_angle_offset));

        // Calculate wall height and vertical position
        int wall_height = (int)(projection_plane_distance / hit_info.distance);
        int wall_start = (window_height / 2) - (wall_height / 2);
        int wall_end = (window_height / 2) + (wall_height / 2);

        // Calculate the position where the wall was hit in world space
        float wallX = -1;
        int texX;
        if (hit_info.type == HitType::VERTICAL) {
            // For vertical walls, use the y-coordinate of the hit
            // wallX = hit_info.intersection.y;
            texX = (int)hit_info.intersection.x % cell_size;
        } else {
            // For horizontal walls, use the x-coordinate of the hit
            // wallX = hit_info.intersection.x;
            texX = (int)hit_info.intersection.y % cell_size;

        }

        // Convert wallX to a value between 0 and 1 (relative to the cell)
        // wallX = fmod(wallX, cell_size) / cell_size;

        // int texX = int(wallX * texWidth);
        // Convert wallX to a texture coordinate
        // ok = ok >= 50 ? 0 : ok;
        // int texX = int(wallX * texWidth) + ok;
        // texX = texX >= 50 ? texX - 50 : texX; 
        // ok++;

        // Debug output for wallX and texX
        std::cout << "Ray " << i << " Intersection: (" << hit_info.intersection.x << ", " << hit_info.intersection.y << ")" << std::endl;
        std::cout << "Ray " << i << " wallX: " << wallX << " texX: " << texX << std::endl;

        // Define the source rectangle from the texture
        SDL_Rect src_rect = { texX, 0, 1, texHeight };

        // Define the destination rectangle on the screen
        int wall_x = (window_width / 2) + i * (window_width / (2 * num_rays));
        int wall_width = (window_width / (2 * num_rays));
        SDL_Rect dest_rect = { wall_x, wall_start, wall_width, wall_height };
        SDL_Texture* tex = wall_texture[hit_info.surface_int - 1];
        // Render the wall slice
        SDL_RenderCopy(RaygineRenderer::GetRenderer(), tex, &src_rect, &dest_rect);

        // Draw ceiling
        SDL_Rect ceil_rect = { dest_rect.x, 0, dest_rect.w, wall_start };
        RaygineRenderer::SetDrawColor(100, 100, 100, 255);
        RaygineRenderer::RenderFillRect(&ceil_rect);

        // Draw floor
        SDL_Rect floor_rect = { dest_rect.x, wall_end, dest_rect.w, window_height - wall_end };
        RaygineRenderer::SetDrawColor(20, 150, 20, 255);
        RaygineRenderer::RenderFillRect(&floor_rect);
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
    SDL_Surface* g_wall_texture_surface = IMG_Load("../_levels/level_1/map/wall_textures/50.png");
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
    SDL_Surface* g_wall_texture_surface1 = IMG_Load("../_levels/level_1/map/wall_textures/ok.png");
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

    SDL_Texture* arr[2];
    arr[0] = wall_texture;
    arr[1] = wall_texture1;

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
        DrawRays(player.pos.x, player.pos.y, player_angle, &player, 200, 65, arr, texWidth, texHeight);
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
