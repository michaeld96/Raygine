#include <SDL2/SDL.h>
#include <iostream>

class RaygineRenderer
{
private:
    // Private constructor to make sure this is not initalized. **SINGLETON**
    RaygineRenderer() {}
    static inline const char* _raygine_window_name = "Raygine";
    static inline SDL_Window* _raygine_window = nullptr;
public:
    // Delete copy constructor and assignment operator to prevent copies.
    RaygineRenderer(const RaygineRenderer&) = delete;
    RaygineRenderer& operator=(const RaygineRenderer&) = delete;
    static void InitWindow(int window_width, int window_height);
    static void DestroyWindow();
};