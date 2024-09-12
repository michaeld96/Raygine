#include <SDL2/SDL.h>
#include <iostream>
#include "vec2.hpp"


namespace Raygine
{
class RaygineRenderer
{
private:
    // Private constructor to make sure this is not initalized. **SINGLETON**
    RaygineRenderer() {}
    static inline const char* _raygine_window_name = "Raygine";
    static inline SDL_Window* _raygine_window = nullptr;
    static inline SDL_Renderer* _raygine_renderer = nullptr;
    static void CheckSDLError(int result, const char* sdl_func_name);
public:
    // Delete copy constructor and assignment operator to prevent copies.
    RaygineRenderer(const RaygineRenderer&) = delete;
    RaygineRenderer& operator=(const RaygineRenderer&) = delete;
    // Most of these are SDL Functions under the hood, but wanted to keep
    // them like this so I don't need to call the renderer every function call.
    static void InitWindow(int window_width, int window_height);
    static SDL_Renderer* GetRenderer();
    static void CreateRenderer();
    static void ClearRenderer();
    static void DestroyRender();
    static void DestroyWindow(); 
    static void SetDrawColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);
    static void RenderFillRect(const SDL_Rect* rect);
    static void RenderDrawLineF(float start_x, float start_y, float end_x, float end_y);
    static void RenderFillRectF(const SDL_FRect* rect);
    static void RenderDrawRect(const SDL_Rect* rect);
    // Custom drawing logic:
    static void DrawPlayer(Vec2<float> player_pos, Vec2<float> player_dir);
};
}