#include "App.hpp"

App::App() {}
App::~App() {}

App &App::GetInstance()
{
    static App *instance = new App();
    return *instance;
}

int App::Init()
{
    // SDL Init
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }
    // IMG Init
    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_WEBP))
    {
        printf("Error: %s\n", IMG_GetError());
        return -1;
    }

    mainScale = ImGui_ImplSDL2_GetContentScaleForDisplay(0);
    windowFlags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    // create window
    mainWindow = SDL_CreateWindow("Laboratorium przetwarzanie obrazów", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)(1280 * mainScale), (int)(720 * mainScale), windowFlags);
    if (mainWindow == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }
    // create renderer
    bool r = Renderer::GetInstance().Init(mainWindow);
    if (!r)
    {
        SDL_Log("Error creating SDL_Renderer!");
        return -1;
    }

    // ImGui Init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // load font
    lato = io->Fonts->AddFontFromFileTTF("./resources/Lato-Regular.ttf");
    if (lato == nullptr)
        return -1;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup scaling
    style = &ImGui::GetStyle();
    style->ScaleAllSizes(mainScale);
    style->FontScaleDpi = mainScale;

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(mainWindow, Renderer::GetInstance().GetRenderer());
    ImGui_ImplSDLRenderer2_Init(Renderer::GetInstance().GetRenderer());

    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // HERE ADD NEW ALGS TO THE VECTOR
    algorithmsAvailable.emplace(algorithmsAvailable.end(), new Negative());
    algorithmsAvailable.emplace(algorithmsAvailable.end(), new Brighten());
    algorithmsAvailable.emplace(algorithmsAvailable.end(), new Contrast());
    algorithmsAvailable.emplace(algorithmsAvailable.end(), new Exponentiation());
    algorithmsAvailable.emplace(algorithmsAvailable.end(), new LeveledHistogram());
    algorithmsAvailable.emplace(algorithmsAvailable.end(), new Binarization());
    algorithmsAvailable.emplace(algorithmsAvailable.end(), new LinearFilter());
    algorithmsAvailable.emplace(algorithmsAvailable.end(), new MedianFilter());
    algorithmsAvailable.emplace(algorithmsAvailable.end(), new Erosion());
    algorithmsAvailable.emplace(algorithmsAvailable.end(), new Dilatation());
    algorithmsAvailable.emplace(algorithmsAvailable.end(), new Skeletonization());
    algorithmsAvailable.emplace(algorithmsAvailable.end(), new Hought());

    return 0;
}

int App::MainLoop()
{
    while (runLoop)
    {
        // for auto refresh
        auto beg = std::chrono::high_resolution_clock::now();

        // handle input
        int eventsCode = HandleEvents();
        if (eventsCode == MINIMIZED)
            continue;
        if (eventsCode == QUIT)
            break;

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // use font
        ImGui::PushFont(lato, 18.0f);

        DrawMenuBar();
        // for resize
        SDL_GetWindowSize(mainWindow, &currWidth, &currHeight);
        DrawPicturesAndMiddle();
        DrawHistogramsAndFunctions();

        if (helpWindowActive)
            DrawHelpMenu();

        ImGui::PopFont();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        Render();

        // calculate time passed
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<float>(end - beg);
        if (counterRefreshImage > 0)
            counterRefreshImage -= duration.count();
        if (counterRefreshImage < 0)
            counterRefreshImage = 0.0;

        // auto refresh
        if (autoRefreshPictureEnabled)
            AutoRefreshOutputImage();

        // special case refresh
        if (currAlgorithm != nullptr)
            if (!currAlgorithm->CanBeAutoRefreshed())
                RefreshSkelAndHought();
    }

    Cleanup();
    return 0;
}

void App::Cleanup()
{
    // Cleanup
    for (u_int64_t it = 0; it < algorithmsAvailable.size(); it++)
        delete algorithmsAvailable[it];
    algorithmsAvailable.clear();
    inputImage.ClearImage();
    outputImage.ClearImage();
    io->Fonts->RemoveFont(lato);
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyWindow(mainWindow);
    IMG_Quit();
    SDL_Quit();
}

int App::HandleEvents()
{
    while (SDL_PollEvent(&event))
    {
        // window closed
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
        {
            runLoop = false;
            return QUIT;
        }
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(mainWindow))
        {
            runLoop = false;
            return QUIT;
        }
    }
    // window minimized
    if (SDL_GetWindowFlags(mainWindow) & SDL_WINDOW_MINIMIZED)
    {
        SDL_Delay(10);
        return MINIMIZED;
    }
    return 0;
}

void App::Render()
{
    // render the window
    Mutex::GetInstance().Lock();
    ImGui::Render();
    SDL_RenderSetScale(Renderer::GetInstance().GetRenderer(), io->DisplayFramebufferScale.x, io->DisplayFramebufferScale.y);
    SDL_SetRenderDrawColor(Renderer::GetInstance().GetRenderer(), (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
    SDL_RenderClear(Renderer::GetInstance().GetRenderer());
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), Renderer::GetInstance().GetRenderer());
    SDL_RenderPresent(Renderer::GetInstance().GetRenderer());
    Mutex::GetInstance().Unlock();
}

void App::DrawMenuBar()
{
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Plik"))
    {
        // save with the same name as input
        if (ImGui::MenuItem("Zapisz", NULL, false, !outputImage.NoSurface()))
        {
            if (FileSelector::GetInstance().FileExists(outputImage.GetImagePath()))
                warningPopupActive = true;
            else
                outputImage.SaveImage();
        }
        // save as a file or with custom name
        if (ImGui::MenuItem("Zapisz jako", NULL, false, !outputImage.NoSurface()))
        {
            saveAsPopupActive = true;
            FileSelector::GetInstance().RefreshCurrDir();
        }
        // load a file
        if (ImGui::MenuItem("Wczytaj"))
        {
            loadPopupActive = true;
            FileSelector::GetInstance().RefreshCurrDir();
        }
        // quit
        ImGui::Separator();
        if (ImGui::MenuItem("Wyjdź"))
            runLoop = false;
        ImGui::EndMenu();
    }

    // all algs
    if (ImGui::BeginMenu("Algorytmy"))
    {
        DrawAlgMenuElements();
        ImGui::EndMenu();
    }

    // settings
    if (ImGui::BeginMenu("Ustawienia"))
    {
        if (currAlgorithm != nullptr)
            ImGui::MenuItem("Automatyczne odświerzanie", NULL, &autoRefreshPictureEnabled, currAlgorithm->CanBeAutoRefreshed());
        else
            ImGui::MenuItem("Automatyczne odświerzanie", NULL, &autoRefreshPictureEnabled, false);

        ImGui::MenuItem("Czas odświerzania", NULL, &settingsPopupActive, autoRefreshPictureEnabled);
        ImGui::EndMenu();
    }

    // help menu and imgui demo
    if (ImGui::BeginMenu("Pomoc"))
    {
        if (ImGui::MenuItem("O programie", NULL, helpWindowActive))
            helpWindowActive = !helpWindowActive;
        ImGui::Separator();
        if (ImGui::MenuItem("Pokaż ImGui Demo", NULL, show_demo_window))
            show_demo_window = !show_demo_window;
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();

    // popups
    if (loadPopupActive)
        DrawLoadPopup();

    if (saveAsPopupActive)
        DrawSavePopup();

    // if save is used
    if (warningPopupActive && !saveAsPopupActive)
        DrawSaveWarningPopup();

    if (settingsPopupActive)
        DrawSettingsPopup();
}

void App::DrawPicturesAndMiddle()
{
    // input image
    float h = ImGui::GetFrameHeight();
    ImGui::SetNextWindowPos(ImVec2(0, h));
    ImGui::SetNextWindowSize(ImVec2((currWidth - MIDDLE_W) / 2, currHeight - MENU_ALG_HIST_H));
    ImGui::Begin("Obraz wejściowy", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize);
    if (!inputImage.NoTexture())
    {
        // if fits in space set it to middle
        if (inputImage.GetWidth() < ImGui::GetWindowWidth())
            ImGui::SameLine((ImGui::GetWindowWidth() - inputImage.GetWidth()) / 2);
        if (inputImage.GetHeight() < ImGui::GetWindowHeight())
            ImGui::SetCursorPosY((ImGui::GetWindowHeight() - inputImage.GetHeight()) / 2);
        ImGui::Image((ImTextureRef)inputImage.GetTexture(), ImVec2(inputImage.GetWidth(), inputImage.GetHeight()));
    }
    ImGui::End();

    // middle buttons
    DrawMiddleButtonsWindow(h);

    // output image
    ImGui::SetNextWindowPos(ImVec2(currWidth / 2 + MIDDLE_W / 2, h));
    ImGui::SetNextWindowSize(ImVec2((currWidth - MIDDLE_W) / 2, currHeight - MENU_ALG_HIST_H));
    ImGui::Begin("Obraz wyjściowy", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize);
    // shared resources
    Mutex::GetInstance().Lock();
    if (!outputImage.NoTexture())
    {
        // if fits in space set it to middle
        if (outputImage.GetWidth() < ImGui::GetWindowWidth())
            ImGui::SameLine((ImGui::GetWindowWidth() - outputImage.GetWidth()) / 2);
        if (outputImage.GetHeight() < ImGui::GetWindowHeight())
            ImGui::SetCursorPosY((ImGui::GetWindowHeight() - outputImage.GetHeight()) / 2);
        ImGui::Image((ImTextureRef)outputImage.GetTexture(), ImVec2(outputImage.GetWidth(), outputImage.GetHeight()));
    }
    Mutex::GetInstance().Unlock();
    ImGui::End();
    // for Hought transormation it has to be drawn a bit diffrent
    // else
    // {
    //     ImGui::Begin("Tablica akumulatorów", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize);
    //     // CS
    //     Mutex::GetInstance().Lock();
    //     ImGui::Text("Wartość maksymalna tablicy akumulatorów: %d", params.maxHoughtVal);
    //     ImGui::Text("Wartość ro: %d", params.maxIndexRo);
    //     ImGui::Text("Wartość theta: %d", params.maxIndexTheta);
    //     ImGui::Separator();
    //     if (!outputImage.NoTexture())
    //     {
    //         // if fits in space set it to middle and below text
    //         if (outputImage.GetWidth() < ImGui::GetWindowWidth())
    //             ImGui::SameLine((ImGui::GetWindowWidth() - outputImage.GetWidth()) / 2);
    //         if (outputImage.GetHeight() < ImGui::GetWindowHeight() - ImGui::GetCursorPosY())
    //             ImGui::SetCursorPosY((ImGui::GetWindowHeight() + ImGui::GetCursorPosY() - outputImage.GetHeight()) / 2);
    //         ImGui::Image((ImTextureRef)outputImage.GetTexture(), ImVec2(outputImage.GetWidth(), outputImage.GetHeight()));
    //     }
    //     Mutex::GetInstance().Unlock();
    //     ImGui::End();
    // }

    if (errorPopupAlgActive)
        DrawMiddleErrorPopup();
}

void App::DrawMiddleButtonsWindow(float h)
{
    ImGui::SetNextWindowPos(ImVec2((currWidth - MIDDLE_W) / 2, h));
    ImGui::SetNextWindowSize(ImVec2(MIDDLE_W, currHeight - MENU_ALG_HIST_H));
    ImGui::Begin("Separator", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
    ImGui::Text("Wybrany algorytm:");
    ImGui::Text("%s", selectedAlgorithmName.c_str());

    ImGui::SeparatorText("Rozpocznij");
    if (ImGui::Button("Przetwórz obraz", ImVec2(MIDDLE_BUTTON_W, MIDDLE_BUTTON_H)))
    {
        // not valid for transformation
        if (currAlgorithm == nullptr || inputImage.NoSurface())
            errorPopupAlgActive = true;
        else
        {
            // can not be called if thread is execiting
            Mutex::GetInstance().ThreadRunning();
            outputImage = inputImage;
            if (autoRefreshPictureEnabled)
            {
                counterRefreshImage = refreshIntervalValue;
                Mutex::GetInstance().SetState(Mutex::WaitingForCounter);
            }
            if (outputImage.NoSurface())
            {
                errorPopupAlgActive = true;
                errorCopying = true;
            }
            // not running
            if (!algorithmThread.joinable())
                // LaunchAlgorithms();
                algorithmThread = std::thread(&Algorithm::AlgorithmFunction, currAlgorithm, &outputImage);
        }
    }

    Mutex::GetInstance().Lock();
    if (Mutex::GetInstance().IsThreadRunning())
        inProgressPopupActive = true;
    Mutex::GetInstance().Unlock();

    if (inProgressPopupActive)
        DrawInProgressPopup();

    ImGui::SeparatorText("Opcje");
    if (ImGui::Button("Parametry", ImVec2(MIDDLE_BUTTON_W, MIDDLE_BUTTON_H)))
        ImGui::OpenPopup("Parametry");
    if (ImGui::Button("Odśwież obraz wyjściowy", ImVec2(MIDDLE_BUTTON_W, MIDDLE_BUTTON_H)))
    {
        // can not be opend if thread is running
        if (!outputImage.NoSurface())
        {
            outputImage.RefreshPixelValuesArrays();
            outputImage.RefreshTexture();
        }
        else
            errorPopupAlgActive = true;
    }

    if (!inputImage.NoSurface())
    {
        ImGui::Separator();
        ImGui::Text("Zamień obraz na odcienie\nszarości");
        if (ImGui::Button("Odcień szarości", ImVec2(MIDDLE_BUTTON_W, MIDDLE_BUTTON_H)))
            inputImage.TurnToGrayScale();
    }

    // only if output image is transformed
    if (!outputImage.NoSurface())
    {
        ImGui::Separator();
        ImGui::Text("Ustaw obraz przetworzony\n(wyjściowy) jako wejściowy");
        if (ImGui::Button("Wejście = Wyjście", ImVec2(MIDDLE_BUTTON_W, MIDDLE_BUTTON_H)))
        {
            // can not be done if thread is running
            inputImage = outputImage;
            outputImage.ClearImage();
        }
    }

    DrawParametersPopup();

    // can not be opend if thread is running
    ImGui::SeparatorText("Reset");
    if (ImGui::Button("Resetuj wybrany algorytm", ImVec2(MIDDLE_BUTTON_W, MIDDLE_BUTTON_H)))
    {
        // algorithmSelected = None;
        selectedAlgorithmName = "Brak wybranego algorytmu";
        currAlgorithm = nullptr;
        outputImage.ClearImage();
        resetDonePopupActive = true;
    }
    // can not be opend if thread is running
    if (ImGui::Button("Resetuj parametry", ImVec2(MIDDLE_BUTTON_W, MIDDLE_BUTTON_H)))
    {
        ResetParameters();
        resetDonePopupActive = true;
    }

    if (resetDonePopupActive)
        DrawResetDonePopup();

    ImGui::End();
}

void App::DrawHistogramsAndFunctions()
{
    // set it to three subwindows
    float h = ImGui::GetFrameHeight() + (currHeight - MENU_ALG_HIST_H);
    float maxO = *(std::max_element(outputImage.GetLightValues(), outputImage.GetLightValues() + 255));
    float maxI = *(std::max_element(inputImage.GetLightValues(), inputImage.GetLightValues() + 255));

    ImGui::SetNextWindowPos(ImVec2(0, h));
    ImGui::SetNextWindowSize(ImVec2(currWidth, HIST_BAR_HEIGHT));
    ImGui::Begin("Funkcje i wykresy", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    float freeSpace = currWidth - 3 * HIST_WINDOW_W;
    freeSpace -= 2 * BORDER_OFFSET;

    // in
    ImGui::SameLine(BORDER_OFFSET);
    ImGui::BeginChild("Histogram wejściowy", ImVec2(HIST_WINDOW_W, HIST_WINDOW_H), ImGuiChildFlags_Borders);
    ImGui::Text("Obraz wejściowy");
    ImGui::RadioButton("Jasność", &modeI, 0);
    ImGui::SameLine();
    ImGui::RadioButton("R", &modeI, 1);
    ImGui::SameLine();
    ImGui::RadioButton("G", &modeI, 2);
    ImGui::SameLine();
    ImGui::RadioButton("B", &modeI, 3);
    if (modeI == Brightnes)
        ImGui::PlotHistogram("##", inputImage.GetLightValues(), MAX_VAL, 0, NULL, 0.0f, maxI + 10, ImVec2(HIST_W, HIST_H));
    if (modeI == R)
        ImGui::PlotHistogram("##", inputImage.GetRValues(), MAX_VAL, 0, NULL, 0.0f, *(std::max_element(inputImage.GetRValues(), inputImage.GetRValues() + 255)) + 10, ImVec2(HIST_W, HIST_H));
    if (modeI == G)
        ImGui::PlotHistogram("##", inputImage.GetGValues(), MAX_VAL, 0, NULL, 0.0f, *(std::max_element(inputImage.GetGValues(), inputImage.GetGValues() + 255)) + 10, ImVec2(HIST_W, HIST_H));
    if (modeI == B)
        ImGui::PlotHistogram("##", inputImage.GetBValues(), MAX_VAL, 0, NULL, 0.0f, *(std::max_element(inputImage.GetBValues(), inputImage.GetBValues() + 255)) + 10, ImVec2(HIST_W, HIST_H));
    ImGui::EndChild();

    // func
    ImGui::SameLine(HIST_WINDOW_W + BORDER_OFFSET + freeSpace / 2);
    ImGui::BeginChild("Funkcja transformacji", ImVec2(HIST_WINDOW_W, HIST_WINDOW_H), ImGuiChildFlags_Borders);
    ImGui::Text("Dystrybuanta Obrazu");
    ImGui::RadioButton("Wejściowego", &modeD, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Wyjściowego", &modeD, 1);
    if (modeD == InputDist)
        ImGui::PlotLines("##", inputImage.GetDistributor(), MAX_VAL, 0, NULL, 0, 255, ImVec2(HIST_W, HIST_H));
    if (modeD == OutputDist)
    {
        Mutex::GetInstance().Lock();
        ImGui::PlotLines("##", outputImage.GetDistributor(), MAX_VAL, 0, NULL, 0, 255, ImVec2(HIST_W, HIST_H));
        Mutex::GetInstance().Unlock();
    }
    ImGui::EndChild();

    // out
    ImGui::SameLine(HIST_WINDOW_W * 2 + BORDER_OFFSET + freeSpace);
    ImGui::BeginChild("Histogram wyjściowy", ImVec2(HIST_WINDOW_W, HIST_WINDOW_H), ImGuiChildFlags_Borders);
    ImGui::Text("Obraz wyjściowy");
    ImGui::RadioButton("Jasność", &modeO, 0);
    ImGui::SameLine();
    ImGui::RadioButton("R", &modeO, 1);
    ImGui::SameLine();
    ImGui::RadioButton("G", &modeO, 2);
    ImGui::SameLine();
    ImGui::RadioButton("B", &modeO, 3);
    // shared resources
    Mutex::GetInstance().Lock();
    if (modeO == Brightnes)
        ImGui::PlotHistogram("##", outputImage.GetLightValues(), MAX_VAL, 0, NULL, 0.0f, maxO + 10, ImVec2(HIST_W, HIST_H));
    if (modeO == R)
        ImGui::PlotHistogram("##", outputImage.GetRValues(), MAX_VAL, 0, NULL, 0.0f, *(std::max_element(outputImage.GetRValues(), outputImage.GetRValues() + 255)) + 10, ImVec2(HIST_W, HIST_H));
    if (modeO == G)
        ImGui::PlotHistogram("##", outputImage.GetGValues(), MAX_VAL, 0, NULL, 0.0f, *(std::max_element(outputImage.GetGValues(), outputImage.GetGValues() + 255)) + 10, ImVec2(HIST_W, HIST_H));
    if (modeO == B)
        ImGui::PlotHistogram("##", outputImage.GetBValues(), MAX_VAL, 0, NULL, 0.0f, *(std::max_element(outputImage.GetBValues(), outputImage.GetBValues() + 255)) + 10, ImVec2(HIST_W, HIST_H));
    Mutex::GetInstance().Unlock();
    ImGui::EndChild();
    ImGui::End();
}

void App::DrawAlgMenuElements()
{
    for (auto alg : algorithmsAvailable)
    {
        if (ImGui::MenuItem(alg->GetName().c_str(), NULL, selectedAlgorithmName == alg->GetName()))
        {
            currAlgorithm = alg;
            selectedAlgorithmName = alg->GetName();
        }
    }
}

void App::DrawLoadPopup()
{
    // can not be opend if thread is running
    ImGui::OpenPopup("WczytajPlik", ImGuiPopupFlags_NoReopen);
    ImGui::SetNextWindowSize(ImVec2(FILE_POPUP_WIDTH, FILE_POPUP_HEIGHT));
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("WczytajPlik", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        auto dir = FileSelector::GetInstance().GetCurrDir();
        auto map = FileSelector::GetInstance().GetDirMaped();
        // curr dir path
        ImGui::BeginChild("Dir", ImVec2(DIR_LIST_WIDTH, DIR_LIST_HEIGHT), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::Text("%s", FileSelector::GetInstance().GetCurrDirectoryPath().c_str());
        ImGui::Separator();
        // display entries as selectebles
        for (auto entry : dir)
            if (ImGui::Selectable(entry.path().filename().c_str(), map[entry.path()], ImGuiSelectableFlags_NoAutoClosePopups))
                if (FileSelector::GetInstance().SelectEntry(entry.path()) == FileSelector::FileEntry)
                {
                    // could not be loaded
                    if (inputImage.SetSourceImage(FileSelector::GetInstance().GetFullPathToEntry()) == -1)
                    {
                        errorPopupActive = true;
                        outputImage.ClearImage();
                    }
                    else
                    {
                        outputImage.ClearImage();
                        loadPopupActive = false;
                        ImGui::CloseCurrentPopup();
                    }
                }
        ImGui::EndChild();
        ImGui::Separator();
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - CANCEL_BUTTON_W / 2);
        if (ImGui::Button("Otwórz", ImVec2(CANCEL_BUTTON_W, 0)))
        {
            if (FileSelector::GetInstance().SelectCurrEntry() == FileSelector::FileEntry)
            {
                // could not be loaded
                if (inputImage.SetSourceImage(FileSelector::GetInstance().GetFullPathToEntry()) == -1)
                {
                    errorPopupActive = true;
                    outputImage.ClearImage();
                }
                else
                {
                    outputImage.ClearImage();
                    loadPopupActive = false;
                    ImGui::CloseCurrentPopup();
                }
            }
        }
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - CANCEL_BUTTON_W / 2);
        if (ImGui::Button("Folder wyżej", ImVec2(CANCEL_BUTTON_W, 0)))
        {
            FileSelector::GetInstance().GoUpADirectory();
        }
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - CANCEL_BUTTON_W / 2);
        if (ImGui::Button("Anuluj", ImVec2(CANCEL_BUTTON_W, 0)))
        {
            loadPopupActive = false;
            ImGui::CloseCurrentPopup();
        }

        if (errorPopupActive)
        {
            ImGui::OpenPopup("BLĄD", ImGuiPopupFlags_NoReopen);
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowSize(ImVec2(0, 100));
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            if (ImGui::BeginPopupModal("BLĄD", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
            {
                ImGui::Text("Nie udało sie wczytać pliku");
                ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - CANCEL_BUTTON_W / 2);
                if (ImGui::Button("OK", ImVec2(CANCEL_BUTTON_W, 0)))
                {
                    errorPopupActive = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
        ImGui::EndPopup();
    }
}

void App::DrawSavePopup()
{
    // can not be opend if thread is running
    ImGui::OpenPopup("ZapiszPlik", ImGuiPopupFlags_NoReopen);
    ImGui::SetNextWindowSize(ImVec2(FILE_POPUP_WIDTH, SAVE_POPUP_HEIGHT));
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("ZapiszPlik", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        auto dir = FileSelector::GetInstance().GetCurrDir();
        auto map = FileSelector::GetInstance().GetDirMaped();
        // curr dir path
        ImGui::BeginChild("Dir", ImVec2(DIR_LIST_WIDTH, DIR_LIST_HEIGHT), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::Text("%s", FileSelector::GetInstance().GetCurrDirectoryPath().c_str());
        ImGui::Separator();
        // display dir as selectables
        for (auto entry : dir)
            if (ImGui::Selectable(entry.path().filename().c_str(), map[entry.path()], ImGuiSelectableFlags_NoAutoClosePopups))
                if (FileSelector::GetInstance().SelectEntry(entry.path()) == FileSelector::FileEntry)
                    warningPopupActive = true;
        ImGui::EndChild();

        ImGui::Separator();

        ImGui::Text("Nazwa pliku");
        ImGui::InputText("wpisz", fileNameBuff, 64);

        const char *ext[] = {".png", ".jpg", ".bmp"};
        ImGui::Text("Rozszerzenie");
        ImGui::Combo("wybierz", &currExtension, ext, IM_ARRAYSIZE(ext));

        ImGui::Text("Zapisz - jeżeli chcemy nadać nazwę");
        ImGui::Text("Wybierz - jeżeli chcemy wybrać \n istniejacy plik lub folder");
        ImGui::Separator();

        int offset = (FILE_POPUP_WIDTH - 2 * CANCEL_BUTTON_W - BUTTON_OFFSET) / 2;

        ImGui::SetCursorPosX(offset);
        if (ImGui::Button("Zapisz", ImVec2(CANCEL_BUTTON_W, 0)))
        {
            // can not be empty
            std::string buffStr = fileNameBuff;
            if (buffStr == "")
                errorPopupActive = true;
            // already exists
            else if (FileSelector::GetInstance().FileExists(FileSelector::GetInstance().GetCurrDirectoryPath().string() + '/' + fileNameBuff + ext[currExtension]))
            {
                warningPopupActive = true;
                customName = true;
            }
            else
            {
                outputImage.SaveImageAs(FileSelector::GetInstance().GetCurrDirectoryPath(), fileNameBuff, currExtension);
                saveAsPopupActive = false;
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::SameLine(offset + CANCEL_BUTTON_W + BUTTON_OFFSET);
        if (ImGui::Button("Wybierz", ImVec2(CANCEL_BUTTON_W, 0)))
        {
            // save as existing
            if (FileSelector::GetInstance().SelectCurrEntry() == FileSelector::FileEntry)
                warningPopupActive = true;
        }
        ImGui::SetCursorPosX(offset);
        if (ImGui::Button("Folder wyżej", ImVec2(CANCEL_BUTTON_W, 0)))
        {
            FileSelector::GetInstance().GoUpADirectory();
        }
        ImGui::SameLine(offset + CANCEL_BUTTON_W + BUTTON_OFFSET);
        if (ImGui::Button("Anuluj", ImVec2(CANCEL_BUTTON_W, 0)))
        {
            saveAsPopupActive = false;
            ImGui::CloseCurrentPopup();
        }
        DrawSaveWarningAndErrorPopup();
        ImGui::EndPopup();
    }
}

void App::DrawSaveWarningAndErrorPopup()
{
    if (warningPopupActive)
    {
        ImGui::OpenPopup("OSTRZEŻENIE", ImGuiPopupFlags_NoReopen);
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        if (ImGui::BeginPopupModal("OSTRZEŻENIE", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
        {
            ImGui::Text("Plik o takie nazwie już istnieje czy chcesz go nadpisać?");
            ImGui::Separator();

            int offset = (ImGui::GetWindowWidth() - 2 * CANCEL_BUTTON_W - BUTTON_OFFSET) / 2;

            ImGui::SetCursorPosX(offset);
            // override
            if (ImGui::Button("Zapisz", ImVec2(CANCEL_BUTTON_W, 0)))
            {
                if (customName)
                {
                    outputImage.SaveImageAs(FileSelector::GetInstance().GetCurrDirectoryPath(), fileNameBuff, currExtension);
                    customName = false;
                }
                else
                    outputImage.SaveImageAs(FileSelector::GetInstance().GetFullPathToEntry());
                saveAsPopupActive = false;
                warningPopupActive = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine(offset + CANCEL_BUTTON_W + BUTTON_OFFSET);
            if (ImGui::Button("Anuluj", ImVec2(CANCEL_BUTTON_W, 0)))
            {
                warningPopupActive = false;
                FileSelector::GetInstance().DeselectCurrEntry();
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (!saveAsPopupActive)
            ImGui::CloseCurrentPopup();
    }

    // can not be empty
    if (errorPopupActive)
    {
        ImGui::OpenPopup("BLĄD", ImGuiPopupFlags_NoReopen);
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowSize(ImVec2(0, POPUP_HEIGHT));
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        if (ImGui::BeginPopupModal("BLĄD", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
        {
            ImGui::Text("Nazwa pliku nie może być pusta");
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - CANCEL_BUTTON_W / 2);
            if (ImGui::Button("OK", ImVec2(CANCEL_BUTTON_W, 0)))
            {
                errorPopupActive = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
}

void App::DrawSaveWarningPopup()
{
    // can not be opend if thread is running
    ImGui::OpenPopup("OSTRZEŻENIE", ImGuiPopupFlags_NoReopen);
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("OSTRZEŻENIE", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        ImGui::Text("Plik o takie nazwie już istnieje czy chcesz go nadpisać?");
        ImGui::Separator();

        int offset = (ImGui::GetWindowWidth() - 2 * CANCEL_BUTTON_W - BUTTON_OFFSET) / 2;

        ImGui::SetCursorPosX(offset);
        // override
        if (ImGui::Button("Zapisz", ImVec2(CANCEL_BUTTON_W, 0)))
        {
            outputImage.SaveImage();
            warningPopupActive = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine(offset + CANCEL_BUTTON_W + BUTTON_OFFSET);
        if (ImGui::Button("Anuluj", ImVec2(CANCEL_BUTTON_W, 0)))
        {
            warningPopupActive = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void App::DrawSettingsPopup()
{
    if (settingsPopupActive)
    {
        ImGui::OpenPopup("Ustawienia", ImGuiPopupFlags_NoReopen);
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowSize(ImVec2(POPUP_WIDTH, POPUP_HEIGHT + 40));
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        if (ImGui::BeginPopupModal("Ustawienia", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
        {
            // how often
            ImGui::Text("Ustaw co ile sekund obraz\nwyjściowy ma się odświerzać");
            ImGui::InputFloat("##", &refreshIntervalValue, 1);
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - CANCEL_BUTTON_W / 2);
            if (ImGui::Button("Powrót", ImVec2(CANCEL_BUTTON_W, 0)))
            {
                settingsPopupActive = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
}

void App::DrawMiddleErrorPopup()
{
    ImGui::OpenPopup("BLĄD", ImGuiPopupFlags_NoReopen);
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowSize(ImVec2(0, POPUP_HEIGHT));
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("BLĄD", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        // no file loaded
        if (inputImage.NoSurface())
            ImGui::Text("Brak wczytanego obrazu");
        // no alg selected
        else if (currAlgorithm == nullptr)
            ImGui::Text("Brak wybranego algorytmu");
        // not transformed
        else if (errorCopying)
            ImGui::Text("Błąd podczas kopiowania obrazu");
        else if (outputImage.NoSurface())
            ImGui::Text("Nie można odświerzyć obraz nie przetworzony");
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - CANCEL_BUTTON_W / 2);
        if (ImGui::Button("OK", ImVec2(CANCEL_BUTTON_W, 0)))
        {
            errorPopupAlgActive = false;
            errorCopying = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void App::DrawInProgressPopup()
{
    // the algorithm thread is running
    ImGui::OpenPopup("Przetwarzanie obrazu");
    ImGui::SetNextWindowSize(ImVec2(POPUP_WIDTH, POPUP_HEIGHT));
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("Przetwarzanie obrazu"))
    {
        Mutex::GetInstance().Lock();
        auto running = Mutex::GetInstance().IsThreadRunning();
        Mutex::GetInstance().Unlock();
        // if running display an option to cancel
        if (running)
        {
            ImGui::SetCursorPosX(10);
            ImGui::ProgressBar(-1.0f * (float)ImGui::GetTime(), ImVec2(180.0f, 0.0f), "Przetwarzanie...");
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - CANCEL_BUTTON_W / 2);
            if (ImGui::Button("Anuluj", ImVec2(CANCEL_BUTTON_W, 0)))
            {
                Mutex::GetInstance().Lock();
                Mutex::GetInstance().ThreadStopped();
                Mutex::GetInstance().Unlock();

                // remember to join
                if (algorithmThread.joinable())
                    algorithmThread.join();

                // refresh
                outputImage.RefreshPixelValuesArrays();
                outputImage.RefreshTexture();

                inProgressPopupActive = false;
                ImGui::CloseCurrentPopup();
            }
        }
        // if not inform that it is done
        else
        {
            // refresh only once after
            if (!justRefreshed)
            {
                justRefreshed = true;
                Mutex::GetInstance().Lock();
                outputImage.RefreshPixelValuesArrays();
                outputImage.RefreshTexture();
                Mutex::GetInstance().Unlock();
            }
            ImGui::Text("Ukończone");
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - CANCEL_BUTTON_W / 2);
            if (ImGui::Button("OK", ImVec2(CANCEL_BUTTON_W, 0)))
            {
                // remember to join
                if (algorithmThread.joinable())
                    algorithmThread.join();
                inProgressPopupActive = false;
                justRefreshed = false;
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndPopup();
    }
}

void App::DrawParametersPopup()
{
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    // can not be opend if thread is running
    if (ImGui::BeginPopupModal("Parametry", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (currAlgorithm == nullptr)
            ImGui::Text("Nie wybrano algorytmu");
        else
            currAlgorithm->ParamsMenu();
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - CANCEL_BUTTON_W / 2);
        if (ImGui::Button("Powrót", ImVec2(CANCEL_BUTTON_W, 0)))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void App::DrawResetDonePopup()
{
    // reset done
    ImGui::OpenPopup("INFORMACJA");
    ImGui::SetNextWindowSize(ImVec2(POPUP_WIDTH, POPUP_HEIGHT));
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("INFORMACJA"))
    {
        ImGui::Text("Zresetowano");
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - CANCEL_BUTTON_W / 2);
        if (ImGui::Button("OK", ImVec2(CANCEL_BUTTON_W, 0)))
        {
            resetDonePopupActive = false;
            ImGui::CloseCurrentPopup();
        }
    }
    ImGui::EndPopup();
}

void App::DrawHelpMenu()
{
    // not finished
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(HELP_WINDOW_WIDTH, HELP_WINDOW_HEIGHT));
    if (ImGui::Begin("O programie", &helpWindowActive, ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::Text("Ta sekcja jest jeszcze do uzupełnienia");
        ImGui::End();
    }
}

void App::ResetParameters()
{
    for (auto alg : algorithmsAvailable)
        alg->ResetToDefaults();
}

void App::AutoRefreshOutputImage()
{
    Mutex::GetInstance().Lock();
    // do not refresh
    if (outputImage.NoTexture() || !Mutex::GetInstance().IsThreadRunning())
    {
        Mutex::GetInstance().Unlock();
        return;
    }

    // notify the thread to update output image
    if (counterRefreshImage == 0 && Mutex::GetInstance().GetState() == Mutex::WaitingForCounter)
    {
        Mutex::GetInstance().SetState(Mutex::AlgorithmThreadRefresh);
        Mutex::GetInstance().Unlock();
        return;
    }

    // image reasigned time to refresh texture
    if (counterRefreshImage == 0 && Mutex::GetInstance().GetState() == Mutex::MainThreadRefresh)
    {
        outputImage.RefreshTexture();
        outputImage.RefreshPixelValuesArrays();
        counterRefreshImage = refreshIntervalValue;
        Mutex::GetInstance().SetState(Mutex::WaitingForCounter);
        Mutex::GetInstance().Unlock();
        return;
    }

    Mutex::GetInstance().Unlock();
}

void App::RefreshSkelAndHought()
{
    Mutex::GetInstance().Lock();
    // do not refresh
    if (outputImage.NoTexture() || !Mutex::GetInstance().IsThreadRunning())
    {
        Mutex::GetInstance().Unlock();
        return;
    }

    // image reasigned time to refresh texture
    if (Mutex::GetInstance().GetState() == Mutex::MainThreadRefresh)
    {
        outputImage.RefreshTexture();
        outputImage.RefreshPixelValuesArrays();
        counterRefreshImage = refreshIntervalValue;
        Mutex::GetInstance().SetState(Mutex::WaitingForCounter);
        Mutex::GetInstance().Unlock();
        return;
    }

    Mutex::GetInstance().Unlock();
}