#include "Renderer.hpp"

Renderer::Renderer() {}
Renderer::~Renderer()
{
    SDL_DestroyRenderer(renderer);
}

Renderer &Renderer::GetInstance()
{
    static Renderer *instance = new Renderer();
    return *instance;
}

bool Renderer::Init(SDL_Window *mainWindow)
{
    renderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    return renderer != nullptr;
}