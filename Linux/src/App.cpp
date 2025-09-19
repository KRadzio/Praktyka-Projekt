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
    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_WEBP))
    {
        printf("Error: %s\n", IMG_GetError());
        return -1;
    }

    mainScale = ImGui_ImplSDL2_GetContentScaleForDisplay(0);
    windowFlags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    mainWindow = SDL_CreateWindow("Praktyka letnia WETI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)(1280 * mainScale), (int)(720 * mainScale), windowFlags);
    if (mainWindow == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }
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
    (void)io;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    lato = io->Fonts->AddFontFromFileTTF("./resources/Lato-Regular.ttf");
    if(lato == nullptr)
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
    return 0;
}

int App::MainLoop()
{
    while (runLoop)
    {
        auto beg = std::chrono::high_resolution_clock::now();

        int eventsCode = HandleEvents();
        if (eventsCode == -2)
            continue;
        if (eventsCode == -1)
            break;

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::PushFont(lato, 18.0f);

        DrawMenuBar();
        SDL_GetWindowSize(mainWindow, &currWidth, &currHeight);
        DrawPictureSpace();
        DrawHistogramsAndFunctions();

        ImGui::PopFont();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);


        Render();

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<float>(end - beg);
        if (counterImage > 0)
            counterImage -= duration.count();
        if (counterImage < 0)
            counterImage = 0.0;

        if (counterHist > 0)
            counterHist -= duration.count();
        if (counterHist < 0)
            counterHist = 0.0;
    }

    return 0;
}

void App::Cleanup()
{
    // Cleanup
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
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
        {
            runLoop = false;
            return -1;
        }
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(mainWindow))
        {
            runLoop = false;
            return -1;
        }
    }
    if (SDL_GetWindowFlags(mainWindow) & SDL_WINDOW_MINIMIZED)
    {
        SDL_Delay(10);
        return -2;
    }
    return 0;
}

void App::DrawMenuBar()
{
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Plik"))
    {
        if (ImGui::MenuItem("Zapisz", NULL, false, !outputImage.NoSurface()))
        {
            if (FileSelector::GetInstance().FileExists(outputImage.GetImageName() + outputImage.GetExtension()))
                warningPopupActive = true;
            else
                outputImage.SaveImage();
        }
        if (ImGui::MenuItem("Zapisz jako", NULL, false, !outputImage.NoSurface()))
        {
            saveAsPopupActive = true;
            FileSelector::GetInstance().RefreshCurrDir();
        }
        if (ImGui::MenuItem("Wczytaj"))
        {
            loadPopupActive = true;
            FileSelector::GetInstance().RefreshCurrDir();
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Wyjdź"))
            runLoop = false;
        ImGui::EndMenu();
    }

    // this one will get longer can be moved to func
    if (ImGui::BeginMenu("Algorytmy"))
    {
        DrawAlgMenuElements();
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Ustawienia"))
    {
        ImGui::MenuItem("Czas odświerzania");
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Pomoc"))
    {
        ImGui::MenuItem("O programie");
        ImGui::Separator();
        if (ImGui::MenuItem("Pokaż ImGui Demo", NULL, show_demo_window))
            show_demo_window = !show_demo_window;
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();

    if (loadPopupActive)
        DrawLoadPopup();

    if (saveAsPopupActive)
        DrawSavePopup();

    if (warningPopupActive && !saveAsPopupActive)
        DrawSaveWarningPopup();
}

void App::DrawPictureSpace()
{

    float h = ImGui::GetFrameHeight();
    ImGui::SetNextWindowPos(ImVec2(0, h));
    ImGui::SetNextWindowSize(ImVec2((currWidth - MIDDLE_W) / 2, currHeight - MENU_ALG_HIST_H));
    ImGui::Begin("Obraz wejściowy", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize);
    if (!inputImage.NoTexture())
    {
        if (inputImage.GetWidth() < ImGui::GetWindowWidth())
            ImGui::SameLine((ImGui::GetWindowWidth() - inputImage.GetWidth()) / 2);
        if (inputImage.GetHeight() < ImGui::GetWindowHeight())
            ImGui::SetCursorPosY((ImGui::GetWindowHeight() - inputImage.GetHeight()) / 2);
        ImGui::Image((ImTextureRef)inputImage.GetTexture(), ImVec2(inputImage.GetWidth(), inputImage.GetHeight()));
    }
    ImGui::End();

    DrawMiddleButtonsWindow(h);

    ImGui::SetNextWindowPos(ImVec2(currWidth / 2 + MIDDLE_W / 2, h));
    ImGui::SetNextWindowSize(ImVec2((currWidth - MIDDLE_W) / 2, currHeight - MENU_ALG_HIST_H));
    ImGui::Begin("Obraz wyjściowy", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize);
    // CS
    Mutex::GetInstance().Lock();
    if (!outputImage.NoTexture())
    {
        // refresh it only if is getting transformed and in intervals
        if (Mutex::GetInstance().IsThreadRunning() && counterImage == 0.0)
        {
            outputImage.RefreshTexture();
            counterImage = REFRESH_INTERVAL;
        }
        if (outputImage.GetWidth() < ImGui::GetWindowWidth())
            ImGui::SameLine((ImGui::GetWindowWidth() - outputImage.GetWidth()) / 2);
        if (outputImage.GetHeight() < ImGui::GetWindowHeight())
            ImGui::SetCursorPosY((ImGui::GetWindowHeight() - outputImage.GetHeight()) / 2);
        ImGui::Image((ImTextureRef)outputImage.GetTexture(), ImVec2(outputImage.GetWidth(), outputImage.GetHeight()));
    }
    Mutex::GetInstance().Unlock();
    ImGui::End();

    if (errorPopupAlgActive)
    {
        ImGui::OpenPopup("BLĄD", ImGuiPopupFlags_NoReopen);
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowSize(ImVec2(0, 100));
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        if (ImGui::BeginPopupModal("BLĄD", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
        {
            // no file loaded
            if (inputImage.NoSurface())
                ImGui::Text("Brak wczytanego obrazu");
            // no alg selected
            else if (algS == None)
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
}

void App::DrawHistogramsAndFunctions()
{
    float h = ImGui::GetFrameHeight() + (currHeight - MENU_ALG_HIST_H);
    float maxO = *(std::max_element(outputImage.GetLightValues(), outputImage.GetLightValues() + 255));
    float maxI = *(std::max_element(inputImage.GetLightValues(), inputImage.GetLightValues() + 255));

    ImGui::SetNextWindowPos(ImVec2(0, h));
    ImGui::SetNextWindowSize(ImVec2(currWidth, HIST_BAR_HEIGHT));
    ImGui::Begin("Histogramy", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    float freeSpace = currWidth - 3 * HIST_WINDOW_W;
    freeSpace -= 2 * BORDER_OFFSET;

    // in
    ImGui::SameLine(BORDER_OFFSET);
    ImGui::BeginChild("Histogram wejsciowy", ImVec2(HIST_WINDOW_W, HIST_WINDOW_H), ImGuiChildFlags_Borders);
    ImGui::Text("Obraz wejściowy");
    ImGui::RadioButton("Jasność", &modeI, 0);
    ImGui::SameLine();
    ImGui::RadioButton("R", &modeI, 1);
    ImGui::SameLine();
    ImGui::RadioButton("G", &modeI, 2);
    ImGui::SameLine();
    ImGui::RadioButton("B", &modeI, 3);
    if (modeI == Brightnes)
        ImGui::PlotHistogram("##", inputImage.GetLightValues(), 256, 0, NULL, 0.0f, maxI + 10, ImVec2(HIST_W, HIST_H));
    if (modeI == R)
        ImGui::PlotHistogram("##", inputImage.GetRValues(), 256, 0, NULL, 0.0f, *(std::max_element(inputImage.GetRValues(), inputImage.GetRValues() + 255)) + 10, ImVec2(HIST_W, HIST_H));
    if (modeI == G)
        ImGui::PlotHistogram("##", inputImage.GetGValues(), 256, 0, NULL, 0.0f, *(std::max_element(inputImage.GetGValues(), inputImage.GetGValues() + 255)) + 10, ImVec2(HIST_W, HIST_H));
    if (modeI == B)
        ImGui::PlotHistogram("##", inputImage.GetBValues(), 256, 0, NULL, 0.0f, *(std::max_element(inputImage.GetBValues(), inputImage.GetBValues() + 255)) + 10, ImVec2(HIST_W, HIST_H));
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
    // CS
    Mutex::GetInstance().Lock();
    // refresh it only if is getting transformed and in intervals
    if (!outputImage.NoSurface() && Mutex::GetInstance().IsThreadRunning() && counterHist == 0.0)
    {
        outputImage.RefreshPixelValuesArrays();
        counterHist = REFRESH_INTERVAL;
    }
    if (modeO == Brightnes)
        ImGui::PlotHistogram("##", outputImage.GetLightValues(), 256, 0, NULL, 0.0f, maxO + 10, ImVec2(HIST_W, HIST_H));
    if (modeO == R)
        ImGui::PlotHistogram("##", outputImage.GetRValues(), 256, 0, NULL, 0.0f, *(std::max_element(outputImage.GetRValues(), outputImage.GetRValues() + 255)) + 10, ImVec2(HIST_W, HIST_H));
    if (modeO == G)
        ImGui::PlotHistogram("##", outputImage.GetGValues(), 256, 0, NULL, 0.0f, *(std::max_element(outputImage.GetGValues(), outputImage.GetGValues() + 255)) + 10, ImVec2(HIST_W, HIST_H));
    if (modeO == B)
        ImGui::PlotHistogram("##", outputImage.GetBValues(), 256, 0, NULL, 0.0f, *(std::max_element(outputImage.GetBValues(), outputImage.GetBValues() + 255)) + 10, ImVec2(HIST_W, HIST_H));
    Mutex::GetInstance().Unlock();
    ImGui::EndChild();
    ImGui::End();
}

void App::DrawAlgMenuElements()
{
    if (ImGui::MenuItem("Negatyw", NULL, algS == Negative))
    {
        algName = "Negatyw";
        algS = Negative;
    }
    if (ImGui::MenuItem("Rozjaśnij", NULL, algS == Brighten))
    {
        algName = "Rozjaśnij";
        algS = Brighten;
    }
    if (ImGui::MenuItem("Kontrast", NULL, algS == Contrast))
    {
        algName = "Kontrast";
        algS = Contrast;
    }
    if (ImGui::MenuItem("Potęgowanie", NULL, algS == Exponentiation))
    {
        algName = "Potęgowanie";
        algS = Exponentiation;
    }
    if (ImGui::MenuItem("Wyrównanie histogramu", NULL, algS == LeveledHistogram))
    {
        algName = "Wyrównanie histogramu";
        algS = LeveledHistogram;
    }
    if (ImGui::MenuItem("Binaryzacja", NULL, algS == Binarization))
    {
        algName = "Binaryzacja";
        algS = Binarization;
    }
    if (ImGui::MenuItem("Filtry Liniowe", NULL, algS == LinearFilter))
    {
        algName = "Filtry Liniowe";
        algS = LinearFilter;
    }
    if (ImGui::MenuItem("Filtry medianowe", NULL, algS == MedianFilter))
    {
        algName = "Filtry medianowe";
        algS = MedianFilter;
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
        auto dir = FileSelector::GetInstance().GetCurrDirEntryNames();
        auto map = FileSelector::GetInstance().GetDirMaped();
        ImGui::BeginChild("Dir", ImVec2(DIR_LIST_WIDTH, DIR_LIST_HEIGHT), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::Text("%s", FileSelector::GetInstance().GetCurrPath().c_str());
        ImGui::Separator();
        for (auto name : dir)
            if (ImGui::Selectable(name.c_str(), map[name], ImGuiSelectableFlags_NoAutoClosePopups))
                if (FileSelector::GetInstance().SelectEntry(name) == FileSelector::FileEntry)
                {

                    if (inputImage.SetSourceImage(FileSelector::GetInstance().GetFullPathToFile()) == -1)
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
                if (inputImage.SetSourceImage(FileSelector::GetInstance().GetFullPathToFile()) == -1)
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
            else if (FileSelector::GetInstance().SelectCurrEntry() == FileSelector::DirEntry)
                FileSelector::GetInstance().GoUpADirectory();
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
        auto dir = FileSelector::GetInstance().GetCurrDirEntryNames();
        auto map = FileSelector::GetInstance().GetDirMaped();
        ImGui::BeginChild("Dir", ImVec2(DIR_LIST_WIDTH, DIR_LIST_HEIGHT), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::Text("%s", FileSelector::GetInstance().GetCurrPath().c_str());
        ImGui::Separator();
        for (auto name : dir)
            if (ImGui::Selectable(name.c_str(), map[name], ImGuiSelectableFlags_NoAutoClosePopups))
                if (FileSelector::GetInstance().SelectEntry(name) == FileSelector::FileEntry)
                    warningPopupActive = true;
        ImGui::EndChild();

        ImGui::Separator();

        ImGui::Text("Nazwa pliku");
        ImGui::InputText("wpisz", fileNameBuff, 64);

        const char *ext[] = {".png", ".jpg", ".bmp"};
        ImGui::Text("Rozszerzenie");
        ImGui::Combo("wybierz", &currExt, ext, IM_ARRAYSIZE(ext));

        ImGui::Text("Zapisz - jezeli chcemy nadac nazwe");
        ImGui::Text("Wybierz jezeli chcemy wybrac \n istniejacy plik lub folder");
        ImGui::Separator();

        int offset = (FILE_POPUP_WIDTH - 2 * CANCEL_BUTTON_W - 20) / 2;

        ImGui::SetCursorPosX(offset);
        if (ImGui::Button("Zapisz", ImVec2(CANCEL_BUTTON_W, 0)))
        {
            std::string buffStr = fileNameBuff;
            if (buffStr == "")
                errorPopupActive = true;
            else if (FileSelector::GetInstance().FileExists(FileSelector::GetInstance().GetCurrPath().string() + '/' + fileNameBuff + ext[currExt]))
            {
                warningPopupActive = true;
                customName = true;
            }
            else
            {
                outputImage.SaveImageAs(FileSelector::GetInstance().GetCurrPath(), fileNameBuff, currExt);
                saveAsPopupActive = false;
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::SameLine(offset + CANCEL_BUTTON_W + 20);
        if (ImGui::Button("Wybierz", ImVec2(CANCEL_BUTTON_W, 0)))
        {
            if (FileSelector::GetInstance().SelectCurrEntry() == FileSelector::FileEntry)
                warningPopupActive = true;
            else if (FileSelector::GetInstance().SelectCurrEntry() == FileSelector::DirEntry)
                FileSelector::GetInstance().GoUpADirectory();
        }
        ImGui::SetCursorPosX(offset);
        if (ImGui::Button("Folder wyżej", ImVec2(CANCEL_BUTTON_W, 0)))
        {
            FileSelector::GetInstance().GoUpADirectory();
        }
        ImGui::SameLine(offset + CANCEL_BUTTON_W + 20);
        if (ImGui::Button("Anuluj", ImVec2(CANCEL_BUTTON_W, 0)))
        {
            saveAsPopupActive = false;
            ImGui::CloseCurrentPopup();
        }

        if (warningPopupActive)
        {
            ImGui::OpenPopup("OSTRZEŻENIE", ImGuiPopupFlags_NoReopen);
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            if (ImGui::BeginPopupModal("OSTRZEŻENIE", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
            {
                ImGui::Text("Plik o takie nazwie już istnieje czy chcesz go nadpisać?");
                ImGui::Separator();

                int offset = (ImGui::GetWindowWidth() - 2 * CANCEL_BUTTON_W - 20) / 2;

                ImGui::SetCursorPosX(offset);
                if (ImGui::Button("Zapisz", ImVec2(CANCEL_BUTTON_W, 0)))
                {
                    if (customName)
                    {
                        outputImage.SaveImageAs(FileSelector::GetInstance().GetCurrPath(), fileNameBuff, currExt);
                        customName = false;
                    }
                    else
                        outputImage.SaveImageAs(FileSelector::GetInstance().GetFullPathToFile());
                    saveAsPopupActive = false;
                    warningPopupActive = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine(offset + CANCEL_BUTTON_W + 20);
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

        if (errorPopupActive)
        {
            ImGui::OpenPopup("BLĄD", ImGuiPopupFlags_NoReopen);
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowSize(ImVec2(0, 100));
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
        ImGui::EndPopup();
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

        int offset = (ImGui::GetWindowWidth() - 2 * CANCEL_BUTTON_W - 20) / 2;

        ImGui::SetCursorPosX(offset);
        if (ImGui::Button("Zapisz", ImVec2(CANCEL_BUTTON_W, 0)))
        {
            outputImage.SaveImage();
            warningPopupActive = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine(offset + CANCEL_BUTTON_W + 20);
        if (ImGui::Button("Anuluj", ImVec2(CANCEL_BUTTON_W, 0)))
        {
            warningPopupActive = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void App::DrawMiddleButtonsWindow(float h)
{
    ImGui::SetNextWindowPos(ImVec2((currWidth - MIDDLE_W) / 2, h));
    ImGui::SetNextWindowSize(ImVec2(MIDDLE_W, currHeight - MENU_ALG_HIST_H));
    ImGui::Begin("Separator", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
    ImGui::Text("Wybrany algorytm:");
    ImGui::Text("%s", algName.c_str());

    ImGui::SeparatorText("Rozpocznij/Przerwij");
    if (ImGui::Button("Przetwórz obraz", ImVec2(MIDDLE_BUTTON_W, MIDDLE_BUTTON_H)))
    {
        // not valid for transformation
        if (algS == None || inputImage.NoSurface())
            errorPopupAlgActive = true;
        else
        {
            // can not be called if thread is execiting (no CS)
            Mutex::GetInstance().ThreadRunning();
            outputImage = inputImage;
            if (outputImage.NoSurface())
            {
                errorPopupAlgActive = true;
                errorCopying = true;
            }
        }

        switch (algS)
        {
        case Negative:
            algThread = std::thread(&Algorithms::CreateNegative, &outputImage);
            algThread.detach();
            break;
        case Brighten:
            algThread = std::thread(&Algorithms::BrightenImage, &outputImage, &params);
            algThread.detach();
            break;
        case Contrast:
            algThread = std::thread(&Algorithms::Contrast, &outputImage, &params);
            algThread.detach();
            break;
        case Exponentiation:
            algThread = std::thread(&Algorithms::Exponentiation, &outputImage, &params);
            algThread.detach();
            break;
        case LeveledHistogram:
            algThread = std::thread(&Algorithms::LevelHistogram, &outputImage);
            algThread.detach();
            break;
        case Binarization:
            algThread = std::thread(&Algorithms::Binarization, &outputImage, &params);
            algThread.detach();
            break;
        case LinearFilter:
            algThread = std::thread(&Algorithms::LinearFilter, &outputImage, &params);
            algThread.detach();
            break;
        case MedianFilter:
            algThread = std::thread(&Algorithms::MedianFilter, &outputImage, &params);
            algThread.detach();
            break;
        default:
            break;
        }
    }

    Mutex::GetInstance().Lock();
    if (Mutex::GetInstance().IsThreadRunning())
        inProgressPopupActive = true;
    Mutex::GetInstance().Unlock();

    if (inProgressPopupActive)
    {
        ImGui::OpenPopup("Przetwarzanie obrazu");
        ImGui::SetNextWindowSize(ImVec2(200, 80));
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        if (ImGui::BeginPopupModal("Przetwarzanie obrazu"))
        {
            Mutex::GetInstance().Lock();
            auto running = Mutex::GetInstance().IsThreadRunning();
            Mutex::GetInstance().Unlock();
            if (running)
            {
                ImGui::SetCursorPosX(10);
                ImGui::ProgressBar(-1.0f * (float)ImGui::GetTime(), ImVec2(180.0f, 0.0f), "Przetwarzanie...");
                ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - CANCEL_BUTTON_W / 2);
                if (ImGui::Button("Anuluj", ImVec2(CANCEL_BUTTON_W, 0)))
                {
                    Mutex::GetInstance().Lock();
                    Mutex::GetInstance().ThreadStopped();
                    // outputImage.RefreshPixelValuesArrays();
                    // outputImage.RefreshTexture();
                    Mutex::GetInstance().Unlock();
                    inProgressPopupActive = false;
                    ImGui::CloseCurrentPopup();
                }
            }
            else
            {
                // tmp solution but it works
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
                    inProgressPopupActive = false;
                    justRefreshed = false;
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::EndPopup();
        }
    }

    ImGui::SeparatorText("Opcje");
    if (ImGui::Button("Parametry", ImVec2(MIDDLE_BUTTON_W, MIDDLE_BUTTON_H)))
        ImGui::OpenPopup("Parametry");
    if (ImGui::Button("Odśwież obraz", ImVec2(MIDDLE_BUTTON_W, MIDDLE_BUTTON_H)))
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
    if (!outputImage.NoSurface())
        if (ImGui::Button("Wejście = Wyjście", ImVec2(MIDDLE_BUTTON_W, MIDDLE_BUTTON_H)))
        { // can not be done if thread is running
            inputImage = outputImage;
            outputImage.ClearImage();
        }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    // can not be opend if thread is running
    if (ImGui::BeginPopupModal("Parametry", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        switch (algS)
        {
        case None:
            ImGui::Text("Nie wybrano algorytmu");
            break;
        case Negative:
            ImGui::Text("Brak parametrów do tego algorytmu");
            break;
        case Brighten:
            ImGui::SliderInt("O ile rozjaśnić?", &params.value, -255, 255);
            break;
        case Contrast:
            ImGui::SliderFloat("O ile zmienić", &params.contrast, 0.1, 5.0);
            break;
        case Exponentiation:
            ImGui::SliderFloat("Alfa", &params.alfa, 0.1, 3.0);
            break;
        case LeveledHistogram:
            ImGui::Text("Brak parametrów do tego algorytmu");
            break;
        case Binarization:

            ImGui::RadioButton("Ręcznie ustaw próg", &params.method, Algorithms::None);
            ImGui::SameLine();
            ImGui::RadioButton("Metoda Gradientowa", &params.method, Algorithms::Gradient);
            ImGui::SameLine();
            ImGui::RadioButton("Metoda iteracyjna", &params.method, Algorithms::Histogram);
            if (params.method == Algorithms::BinarizationMethod::None)
            {
                ImGui::RadioButton("Jeden próg", &params.boundCount, 1);
                ImGui::SameLine();
                ImGui::RadioButton("Dwa progi", &params.boundCount, 2);
                ImGui::SliderInt("t", &params.lowerBound, 0, 255);
                if (params.boundCount == 2)
                    ImGui::SliderInt("t1", &params.upperBound, 0, 255);
            }
            break;
        case LinearFilter:

            ImGui::RadioButton("3x3", &params.linearFilterSize, Algorithms::MatrixSize::S3x3);
            ImGui::SameLine();
            ImGui::RadioButton("5x5", &params.linearFilterSize, Algorithms::MatrixSize::S5x5);
            ImGui::SameLine();
            ImGui::RadioButton("7x7", &params.linearFilterSize, Algorithms::MatrixSize::S7x7);

            if (ImGui::RadioButton("Uśredniający", &params.linerFilterS, Algorithms::LinearFilters::Average))
            {
                params.linearMask3x3 = AVERAGE_3x3;
                params.linearMask5x5 = AVERAGE_5x5;
                params.linearMask7x7 = AVERAGE_7x7;
            }
            ImGui::SameLine();

            if (ImGui::RadioButton("Gauss", &params.linerFilterS, Algorithms::LinearFilters::Gauss))
            {
                params.linearMask3x3 = GAUSS_3x3;
                params.linearMask5x5 = GAUSS_5x5;
                params.linearMask7x7 = GAUSS_7x7;
            }
            ImGui::SameLine();

            if (ImGui::RadioButton("Sobel Poziomy", &params.linerFilterS, Algorithms::LinearFilters::SobelHorizontal))
            {
                params.linearMask3x3 = SOBEL_HORIZONTAL_3x3;
                params.linearMask5x5 = SOBEL_HORIZONTAL_5x5;
                params.linearMask7x7 = SOBEL_HORIZONTAL_7x7;
            }
            // in new line
            if (ImGui::RadioButton("Sobel Pionowy", &params.linerFilterS, Algorithms::LinearFilters::SobelVertical))
            {
                params.linearMask3x3 = SOBEL_VERTICAL_3x3;
                params.linearMask5x5 = SOBEL_VERTICAL_5x5;
                params.linearMask7x7 = SOBEL_VERTICAL_7x7;
            }
            ImGui::SameLine();

            if (ImGui::RadioButton("Laplasjan", &params.linerFilterS, Algorithms::LinearFilters::Laplasjan))
            {
                params.linearMask3x3 = LAPLACIAN_3x3;
                params.linearMask5x5 = LAPLACIAN_5x5;
                params.linearMask7x7 = LAPLACIAN_7x7;
            }
            ImGui::SameLine();

            if (ImGui::RadioButton("Wyostrzający", &params.linerFilterS, Algorithms::LinearFilters::Sharpening))
            {
                params.linearMask3x3 = SHARPENING_3x3;
                params.linearMask5x5 = SHARPENING_5x5;
                params.linearMask7x7 = SHARPENING_7x7;
            }

            ImGui::RadioButton("Własna", &params.linerFilterS, Algorithms::LinearFilters::CustomL);
            if (params.linerFilterS == Algorithms::LinearFilters::CustomL)
            {
                ImGui::Text("To może mieć ciekawe efekty");
                DrawLinearInputArray();
            }
            else
                DrawLinearDisplayArray();

            break;
        case MedianFilter:
            ImGui::RadioButton("3x3", &params.medianFilterSize, Algorithms::MatrixSize::S3x3);
            ImGui::SameLine();
            ImGui::RadioButton("5x5", &params.medianFilterSize, Algorithms::MatrixSize::S5x5);
            ImGui::SameLine();
            ImGui::RadioButton("7x7", &params.medianFilterSize, Algorithms::MatrixSize::S7x7);

            if (ImGui::RadioButton("Pełny", &params.medianFilterS, Algorithms::MedianFilters::Full))
            {
                params.medianMask3x3 = MEDIAN_3x3;
                params.medianMask5x5 = MEDIAN_5x5;
                params.medianMask7x7 = MEDIAN_7x7;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Krzyżowy", &params.medianFilterS, Algorithms::MedianFilters::Cross))
            {
                params.medianMask3x3 = MEDIAN_CROSS_3x3;
                params.medianMask5x5 = MEDIAN_CROSS_5x5;
                params.medianMask7x7 = MEDIAN_CROSS_7x7;
            }
            ImGui::SameLine();
            ImGui::RadioButton("Własna", &params.medianFilterS, Algorithms::MedianFilters::CustomM);
            if (params.medianFilterS == Algorithms::MedianFilters::CustomM)
            {
                ImGui::Text("To może mieć ciekawe efekty");
                DrawMedianInputArray();
            }
            else
                DrawMedianDisplayArray();
            break;
        }
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - CANCEL_BUTTON_W / 2);
        if (ImGui::Button("Powrót", ImVec2(CANCEL_BUTTON_W, 0)))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    // can not be opend if thread is running
    ImGui::SeparatorText("Reset");
    if (ImGui::Button("Resetuj wybrany algorytm", ImVec2(MIDDLE_BUTTON_W, MIDDLE_BUTTON_H)))
    {
        algS = None;
        algName = "Brak wybranego algorytmu";
        outputImage.ClearImage();
    }
    // can not be opend if thread is running
    if (ImGui::Button("Resetuj parametry", ImVec2(MIDDLE_BUTTON_W, MIDDLE_BUTTON_H)))
    {
        params.value = 0;
        params.contrast = 1.0;
        params.alfa = 1.0;
        params.boundCount = 1;
        params.lowerBound = 0;
        params.upperBound = 0;
        params.method = Algorithms::BinarizationMethod::None;
        // linear
        params.linerFilterS = Algorithms::LinearFilters::Average;
        params.linearFilterSize = Algorithms::MatrixSize::S3x3;
        params.linearMask3x3 = AVERAGE_3x3;
        params.linearMask5x5 = AVERAGE_5x5;
        params.linearMask7x7 = AVERAGE_7x7;
        // median
        params.medianFilterS = Algorithms::MedianFilters::Full;
        params.medianFilterSize = Algorithms::MatrixSize::S3x3;
        params.medianMask3x3 = MEDIAN_3x3;
        params.medianMask5x5 = MEDIAN_5x5;
        params.medianMask7x7 = MEDIAN_7x7;
    }

    ImGui::End();
}

void App::Render()
{
    Mutex::GetInstance().Lock();
    ImGui::Render();
    SDL_RenderSetScale(Renderer::GetInstance().GetRenderer(), io->DisplayFramebufferScale.x, io->DisplayFramebufferScale.y);
    SDL_SetRenderDrawColor(Renderer::GetInstance().GetRenderer(), (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
    SDL_RenderClear(Renderer::GetInstance().GetRenderer());
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), Renderer::GetInstance().GetRenderer());
    SDL_RenderPresent(Renderer::GetInstance().GetRenderer());
    Mutex::GetInstance().Unlock();
}

void App::DrawLinearInputArray()
{
    if (ImGui::BeginTable("Maska", params.linearFilterSize, ImGuiTableFlags_Borders))
    {
        for (int row = 0; row < params.linearFilterSize; row++)
        {

            ImGui::PushID(row);
            ImGui::TableNextRow();
            for (int col = 0; col < params.linearFilterSize; col++)
            {
                ImGui::TableSetColumnIndex(col);
                ImGui::PushItemWidth(ARRAY_INPUT_WIDTH);
                std::string s = "##" + std::to_string(col);
                if (params.linearFilterSize == Algorithms::MatrixSize::S3x3)
                    ImGui::InputInt(s.c_str(), &params.linearMask3x3[row][col]);
                else if (params.linearFilterSize == Algorithms::MatrixSize::S5x5)
                    ImGui::InputInt(s.c_str(), &params.linearMask5x5[row][col]);
                else
                    ImGui::InputInt(s.c_str(), &params.linearMask7x7[row][col]);
            }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
}

void App::DrawLinearDisplayArray()
{
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - (ARRAY_FIELD_WIDTH * params.linearFilterSize) / 2);
    if (ImGui::BeginTable("Maska", params.linearFilterSize, ImGuiTableFlags_Borders, ImVec2(ARRAY_FIELD_WIDTH * params.linearFilterSize, 0)))
    {
        for (int i = 0; i < params.linearFilterSize; i++)
            ImGui::TableSetupColumn("##", ImGuiTableColumnFlags_WidthFixed, ARRAY_ITEM_WIDTH);
        for (int row = 0; row < params.linearFilterSize; row++)
        {
            ImGui::TableNextRow();
            for (int col = 0; col < params.linearFilterSize; col++)
            {
                ImGui::TableSetColumnIndex(col);
                if (params.linearFilterSize == Algorithms::MatrixSize::S3x3)
                    ImGui::Text("%d", params.linearMask3x3[row][col]);
                else if (params.linearFilterSize == Algorithms::MatrixSize::S5x5)
                    ImGui::Text("%d", params.linearMask5x5[row][col]);
                else
                    ImGui::Text("%d", params.linearMask7x7[row][col]);
            }
        }
        ImGui::EndTable();
    }
}

void App::DrawMedianInputArray()
{
    if (ImGui::BeginTable("Maska", params.medianFilterSize, ImGuiTableFlags_Borders))
    {
        for (int row = 0; row < params.medianFilterSize; row++)
        {

            ImGui::PushID(row);
            ImGui::TableNextRow();
            for (int col = 0; col < params.medianFilterSize; col++)
            {
                ImGui::TableSetColumnIndex(col);
                ImGui::PushItemWidth(ARRAY_INPUT_WIDTH);
                std::string s = "##" + std::to_string(col);
                if (params.medianFilterSize == Algorithms::MatrixSize::S3x3)
                    ImGui::Checkbox(s.c_str(), &params.medianMask3x3[row][col]);
                else if (params.medianFilterSize == Algorithms::MatrixSize::S5x5)
                    ImGui::Checkbox(s.c_str(), &params.medianMask5x5[row][col]);
                else
                    ImGui::Checkbox(s.c_str(), &params.medianMask7x7[row][col]);
            }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
}

void App::DrawMedianDisplayArray()
{
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - (ARRAY_FIELD_WIDTH * params.medianFilterSize) / 2);
    if (ImGui::BeginTable("Maska", params.medianFilterSize, ImGuiTableFlags_Borders, ImVec2(ARRAY_FIELD_WIDTH * params.medianFilterSize, 0)))
    {
        for (int i = 0; i < params.medianFilterSize; i++)
            ImGui::TableSetupColumn("##", ImGuiTableColumnFlags_WidthFixed, ARRAY_ITEM_WIDTH);
        for (int row = 0; row < params.medianFilterSize; row++)
        {
            ImGui::TableNextRow();
            for (int col = 0; col < params.medianFilterSize; col++)
            {
                ImGui::TableSetColumnIndex(col);
                if (params.medianFilterSize == Algorithms::MatrixSize::S3x3)
                    ImGui::Text("%d", params.medianMask3x3[row][col]);
                else if (params.medianFilterSize == Algorithms::MatrixSize::S5x5)
                    ImGui::Text("%d", params.medianMask5x5[row][col]);
                else
                    ImGui::Text("%d", params.medianMask7x7[row][col]);
            }
        }
        ImGui::EndTable();
    }
}

