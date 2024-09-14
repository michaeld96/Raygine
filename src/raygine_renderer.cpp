#include "../include/raygine_renderer.hpp"
namespace Raygine
{
void RaygineRenderer::SetCellSize(int in_cell_size)
{
    _cell_size = in_cell_size;
}
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

SDL_Renderer* RaygineRenderer::GetRenderer()
{
    return _raygine_renderer;
}

void RaygineRenderer::CreateRenderer()
{
    _raygine_renderer = SDL_CreateRenderer( _raygine_window,
                                            -1, 
                                            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (_raygine_renderer == nullptr)
    {
        std::cerr << "Error initializing SDL Renderer!\n";
        exit(1); 
    }
}

void RaygineRenderer::ClearRenderer()
{
    SDL_RenderClear(_raygine_renderer);
}

void RaygineRenderer::DestroyRender()
{
    SDL_DestroyRenderer(_raygine_renderer);
}

void RaygineRenderer::DestroyWindow()
{
    SDL_DestroyWindow(_raygine_window);
}

void RaygineRenderer::SetDrawColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
{
    int result = SDL_SetRenderDrawColor(_raygine_renderer, red, green, blue, alpha);
    CheckSDLError(result, "SDL_SetRenderDrawColor");
}

void RaygineRenderer::RenderFillRect(const SDL_Rect* rect)
{
    int result = SDL_RenderFillRect(_raygine_renderer, rect);
    CheckSDLError(result, "SDL_RenderFillRect");
}

void RaygineRenderer::RenderDrawLineF(float start_x, float start_y, float end_x, float end_y)
{
    int result = SDL_RenderDrawLineF(_raygine_renderer, start_x, start_y, end_x, end_y);
    CheckSDLError(result, "SDL_RenderDrawLineF");
}

void RaygineRenderer::RenderFillRectF(const SDL_FRect* rect)
{
    int result = SDL_RenderFillRectF(_raygine_renderer, rect);
    CheckSDLError(result, "SDL_RenderDrawRectF");
}

void RaygineRenderer::RenderDrawRect(const SDL_Rect* rect)
{
    int result = SDL_RenderDrawRect(_raygine_renderer, rect);
    CheckSDLError(result, "SDL_RenderDrawRect");
}
void RaygineRenderer::CheckSDLError(int result, const char* sdl_func_name)
{
    if (result < 0)
    {
        std::cerr << sdl_func_name << " failed!\n";
        std::cerr << "SDL's reason: " << SDL_GetError() << "\n";
        exit(1);
    }
}

void RaygineRenderer::DrawPlayer(Vec2<float> player_pos, Vec2<float> player_dir)
{
    SDL_FRect player_r = {
        player_pos.x, 
        player_pos.y, 
        5, 
        5 
    };
    RaygineRenderer::SetDrawColor(255, 0, 0, 255);
    RaygineRenderer::RenderFillRectF(&player_r);

    float line_length = 20.0f; // Length of the direction line.
    Vec2<float> end_pos = player_pos + player_dir * line_length;

    // Draw the direction line.
    RaygineRenderer::SetDrawColor(255, 0, 0, 255);
    RaygineRenderer::RenderDrawLineF(player_pos.x, player_pos.y, end_pos.x, end_pos.y);
}

void RaygineRenderer::DrawMap(Map& map)
{
    size_t map_width = map.GetMapWidth();
    size_t map_height = map.GetMapHeight();
    for (size_t y = 0; y < map_width; ++y)
    {
        for (size_t x = 0; x < map_height; ++x)
        {
            SDL_Rect r = { 
                static_cast<int>(x) * _cell_size, 
                static_cast<int>(y) * _cell_size, 
                _cell_size, 
                _cell_size
            };
            if (map.GetMapValue(y, x) == 1)
            {
                // Draw color for the map overhead at the moment is not configurable.
                // TODO: do this! ^
                SetDrawColor(255, 255, 255, 255);
                RenderFillRect(&r);
            }
            // Draw border around all the squares (gives us grid lines).
            SetDrawColor(120, 120, 120, 255);
            RenderDrawRect(&r);
        }
    }
}

}