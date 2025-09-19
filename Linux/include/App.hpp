#ifndef APP_HPP
#define APP_HPP

#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>
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
#define FILE_POPUP_HEIGHT 340
#define SAVE_POPUP_HEIGHT 480
#define DIR_LIST_WIDTH 290
#define DIR_LIST_HEIGHT 200
#define REFRESH_INTERVAL 15.0
#define ARRAY_INPUT_WIDTH 100
#define ARRAY_ITEM_WIDTH 30
#define ARRAY_FIELD_WIDTH 40

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
        LeveledHistogram,
        Binarization,
        LinearFilter,
        MedianFilter,
        Erosion,
        Dilatation
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
    void DrawLinearInputArray();
    void DrawLinearDisplayArray();
    void DrawMedianInputArray();
    void DrawMedianDisplayArray();

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
    bool justRefreshed = false;
    bool errorCopying = false;

    // add more algs (in progress)
    // change how extension is checked in image?
    // add new directory button in save as window
    // add a setting to how often to refresh the transformed image
    // add a transformation function diragram in the middle of the bottom bar
    // make a help window
    // resize some windows
    // change how critical section is handled

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
    float counterImage = 0.0;
    float counterHist = 0.0;

    // Params
    Algorithms::ParametersStruct params; // CS

    // Images
    Image inputImage;
    Image outputImage; // CS

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
    ImFont* lato = nullptr;
};

#endif
