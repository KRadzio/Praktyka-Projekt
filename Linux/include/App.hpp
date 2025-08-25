#ifndef APP_HPP
#define APP_HPP

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <opencv2/opencv.hpp>

#define ALG_BAR_H 40
#define POPUP_SIZE 200
#define MENU_ALG_HIST_H 359
#define HIST_BAR_HEIGHT 300
#define HIST_WINDOW_W 276
#define HIST_WINDOW_H 260
#define BORDER_OFFSET 20
#define HIST_W 256
#define HIST_H 200

class App
{

public:
    static App &GetInstance();
    int Init();
    int MainLoop();

private:
    App();
    ~App();

    // helper functions
private:
    void Cleanup();
    int HandleEvents();
    void DrawMenuBar();
    void DrawAlgorihmsBar();
    void DrawPictureSpace();
    void DrawHistogramsAndFunctions(float arr[], int vc);
    void Render();

private:
    // App state
    bool show_demo_window = true;
    bool show_another_window = false;
    int currWidth = 1280;
    int currHeight = 720;
    bool showAl1 = false;
    bool runLoop = true;

    // Tmp

    // Image
    SDL_Surface *surface = nullptr;
    SDL_Texture *tx = nullptr;
    SDL_Texture *txO = nullptr;
    int texW = 0;
    int texH = 0;

    // Scale
    float mainScale;

    // Events
    SDL_Event event;

    // Rendering
    SDL_WindowFlags windowFlags;
    SDL_Window *mainWindow = nullptr;
    SDL_Renderer *renderer = nullptr;
    ImVec4 clear_color;

    // ImGui
    ImGuiIO *io = nullptr;
    ImGuiStyle *style = nullptr;
};

#endif
