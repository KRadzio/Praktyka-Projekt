#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SDL2/SDL.h>

#ifdef __linux__

#include <SDL2/SDL.h>

#elif _WIN32

#include <SDL.h>

#endif

// singleton
class Renderer
{
public:
    static Renderer &GetInstance();
    // create renderer for window
    bool Init(SDL_Window *mainWindow);
    inline SDL_Renderer *GetRenderer() { return renderer; }

private:
    Renderer();
    ~Renderer();

private:
    SDL_Renderer *renderer = nullptr;
};

#endif
