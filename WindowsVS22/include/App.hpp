#ifndef APP_HPP
#define APP_HPP

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <stdio.h>
#include <SDL.h>
#include <opencv2/opencv.hpp>

#define ALG_BAR_H 40
#define POPUP_SIZE 200
#define MENU_ALG_HIST_H 369
#define HIST_BAR_HEIGHT 310
#define HIST_WINDOW_W 276
#define HIST_WINDOW_H 270
#define BORDER_OFFSET 20
#define HIST_W 256
#define HIST_H 200

class App
{
public:
    enum HistMode
    {
        Brightnes,
        R,
        G,
        B
    };

public:
    static App& GetInstance();
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
    void CreateNegative();
    void BrightenImage();
    void ClearOutputImage();

private:
    // App state
    bool show_demo_window = true;
    bool show_another_window = false;
    int currWidth = 1280;
    int currHeight = 720;
    bool showAl1 = false;
    bool runLoop = true;

    // Tmp
    int value = 0;

    // Abstracy image as class
    // Move transformations to Algorithms
    // Loading files in file loader
    // Gui class may not be needed

    // Image Flags
    int modeI = Brightnes;
    int modeO = Brightnes;

    // Image
    SDL_Surface* surface = nullptr;
    SDL_Surface* surfaceO = nullptr;
    SDL_Texture* tx = nullptr;
    SDL_Texture* txO = nullptr;
    int texW = 0;
    int texH = 0;
    float lightValuesI[256];
    float lightValuesO[256];
    float valuesRI[256];
    float valuesGI[256];
    float valuesBI[256];
    float valuesRO[256];
    float valuesGO[256];
    float valuesBO[256];

    // Scale
    float mainScale;

    // Events
    SDL_Event event;

    // Rendering
    SDL_WindowFlags windowFlags;
    SDL_Window* mainWindow = nullptr;
    SDL_Renderer* renderer = nullptr;
    ImVec4 clear_color;

    // ImGui
    ImGuiIO* io = nullptr;
    ImGuiStyle* style = nullptr;
};

#endif
