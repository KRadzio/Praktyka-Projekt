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
#include "Renderer.hpp"
#include "Mutex.hpp"

#define POPUP_SIZE 200
#define MENU_ALG_HIST_H 329
#define HIST_BAR_HEIGHT 310
#define HIST_WINDOW_W 286
#define HIST_WINDOW_H 270
#define BORDER_OFFSET 20
#define HIST_W 266
#define HIST_H 200
#define MIDDLE_W 200
#define MIDDLE_BUTTON_W 180
#define MIDDLE_BUTTON_H 30
#define CANCEL_BUTTON_W 120
#define FILE_POPUP_WIDTH 300
#define FILE_POPUP_HEIGHT 310
#define SAVE_POPUP_HEIGHT 450
#define DIR_LIST_WIDTH 290
#define DIR_LIST_HEIGHT 200

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
    void DrawPictureSpace();
    void DrawHistogramsAndFunctions();
    void DrawAlgMenuElements();
    void DrawLoadPopup();
    void DrawSavePopup();
    void DrawSaveWarningPopup();
    void DrawMiddleButtonsWindow(float h);
    void Render();

private:
    // App
    // flags
    bool show_demo_window = false;
    bool runLoop = true;
    bool loadPopupActive = false;
    bool saveAsPopupActive = false;
    bool errorPopupActive = false;
    bool errorPopupAlgActive = false;
    bool warningPopupActive = false;
    bool customName = false;
    bool inProgressPopupActive = false;

    // add execution of algs in a separate thread and add a mutex class
    // split some functions in app
    // add more algs
    // change how extension is checked in image?

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

    // File name and extension
    char fileNameBuff[64];
    int currExt = 0;

    // Thread
    std::thread algThread;

    // Params
    Algorithms::ParametersStruct params;

    // Images
    Image inputImage;
    Image outputImage;

    // Scale
    float mainScale;

    // Events
    SDL_Event event;

    // Window
    SDL_WindowFlags windowFlags;
    SDL_Window *mainWindow = nullptr;
    ImVec4 clear_color;

    // ImGui
    ImGuiIO *io = nullptr;
    ImGuiStyle *style = nullptr;
};

#endif
