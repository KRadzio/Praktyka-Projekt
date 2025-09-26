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

// window size
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

// events
#define QUIT -1
#define MINIMIZED -2

// bottom bar
#define MENU_ALG_HIST_H 329
#define HIST_BAR_HEIGHT 310
#define HIST_WINDOW_W 286
#define HIST_WINDOW_H 270
#define BORDER_OFFSET 20
#define HIST_W 266
#define HIST_H 200

// middle menu
#define MIDDLE_W 200
#define MIDDLE_BUTTON_W 180
#define MIDDLE_BUTTON_H 30
#define CANCEL_BUTTON_W 120

// popups
#define POPUP_WIDTH 200
#define POPUP_HEIGHT 100
#define FILE_POPUP_WIDTH 300
#define FILE_POPUP_HEIGHT 340
#define SAVE_POPUP_HEIGHT 480
#define BUTTON_OFFSET 20

// dir items
#define DIR_LIST_WIDTH 290
#define DIR_LIST_HEIGHT 200

// arrays
#define ARRAY_INPUT_WIDTH 100
#define ARRAY_ITEM_WIDTH 30
#define ARRAY_FIELD_WIDTH 40

// help window
#define HELP_WINDOW_WIDTH 400
#define HELP_WINDOW_HEIGHT 300

#define DEFAULT_REFRESH_INTERVAL 5.0

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
        Dilatation,
        Skeletonization,
        Hought
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
    void Render();

    // main parts
    void DrawMenuBar();
    void DrawPicturesAndMiddle();
    void DrawMiddleButtonsWindow(float h);
    void DrawHistogramsAndFunctions();

    // menus
    void DrawAlgMenuElements();

    // popups
    void DrawLoadPopup();
    void DrawSavePopup();
    void DrawSaveWarningAndErrorPopup();
    void DrawSaveWarningPopup();
    void DrawSettingsPopup();
    void DrawMiddleErrorPopup();
    void DrawInProgressPopup();
    void DrawParametersPopup();
    void DrawResetDonePopup();

    // parameters popup split
    void DrawBinarizationParams();
    void DrawLinearFilterParams();
    void DrawMedianFilterParams();
    void DrawErosionParams();
    void DrawDilatationParams();

    // other
    void DrawHelpMenu();
    void LaunchAlgorithms();
    void ResetParameters();
    void AutoRefreshOutputImage();
    void RefreshSkelAndHought();

    // input and display arrays
    void DrawLinearInputArray();
    void DrawLinearDisplayArray();
    void DrawMedianDisplayArray();
    void DrawInputArray(std::string name, int size, std::array<std::array<bool, 3>, 3> &a3x3, std::array<std::array<bool, 5>, 5> &a5x5, std::array<std::array<bool, 7>, 7> &a7x7);

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
    bool autoRefreshPictureEnabled = false;
    bool settingsPopupActive = false;
    bool helpWindowActive = false;
    bool drawExtraFunctionDiagram = false;
    bool resetDonePopupActive = false;

    // TODO
    // rename some methods (there is more than just drawing in some parts)
    // write docs in code, in pdf and in help menu

    // image histograms and plot flags
    int modeI = Brightnes;
    int modeO = Brightnes;
    int modeD = InputDist;

    // window
    int currWidth = WINDOW_HEIGHT;
    int currHeight = WINDOW_HEIGHT;

    // algorithm state
    std::string selectedAlgorithmName = "Brak wybranego algorytmu";
    int algorithmSelected = None;

    // File name and extension
    char fileNameBuff[64];
    int currExtension = 0;

    // Thread
    std::thread algorithmThread;
    float counterRefreshImage = 0.0;
    float refreshIntervalValue = DEFAULT_REFRESH_INTERVAL;

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
    ImFont *lato = nullptr;
};

#endif
