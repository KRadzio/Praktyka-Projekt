#ifndef APP_HPP
#define APP_HPP

#include <iostream>
#include <algorithm>
#include <thread>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "Image.hpp"
#include "FileSelector.hpp"
#include "Algorithms.hpp"

#define ALG_BAR_H 40
#define POPUP_SIZE 200
#define MENU_ALG_HIST_H 369
#define HIST_BAR_HEIGHT 310
#define HIST_WINDOW_W 276
#define HIST_WINDOW_H 270
#define BORDER_OFFSET 20
#define HIST_W 256
#define HIST_H 200
#define MIDDLE_W 200
#define MIDDLE_BUTTON_W 180
#define MIDDLE_BUTTON_H 30
#define CANCEL_BUTTON_W 120
#define FILE_POPUP_WIDTH 300
#define FILE_POPUP_HEIGHT 300

enum HistMode
{
    Brightnes,
    R,
    G,
    B
};

enum DistMode
{
    InputDist,
    OutputDist
};

enum AlgSelected
{
    None,
    Negative,
    Brighten,
    Contrast,
    Exponentiation,
    LeveledHistogram
};

class App
{
public:
    static App &GetInstance();
    int Init();
    int MainLoop();

private:
    App();
    ~App();

private:
    void Cleanup();
    int HandleEvents();
    void DrawMenuBar();
    void DrawAlgorihmsBar();
    void DrawPictureSpace();
    void DrawHistogramsAndFunctions();
    void Render();

private:
    // App
    // flags
    bool show_demo_window = true;
    bool show_another_window = false;
    bool runLoop = true;
    bool loadPopupActive = false;
    bool saveAsPopupActive = false;

    // image histograms and plot flags
    int modeI = Brightnes;
    int modeO = Brightnes;
    int modeD = InputDist;

    // window
    int currWidth = 1280;
    int currHeight = 720;

    // algorithm state
    std::string algName = "Brak wybranego algorytmu";
    int algS = None;

    char fileNameBuff[64];

    // display an error when invalid file is loaded (make an erroe popup)
    // display a warning popup if overriding files, and if quiting without saving
    // improve the ui for load/save (partial)
    // improve saving image
    // add other things that may be needed in image
    // change the way params are handled
    // if App.cpp is to long move part of the gui stuff to Gui.hpp and Gui.cpp

    // Tmp
    int value = 0;
    float contrast = 1.0;
    float alfa = 0.0;

    // Image
    Image inputImage;
    Image outputImage;

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
