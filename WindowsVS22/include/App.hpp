#ifndef APP_HPP
#define APP_HPP

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <opencv2/opencv.hpp>

class App
{

public:
    static App& GetInstance();
    int MainLoop();

private:
    App();
    ~App();

private:
    

};

#endif
