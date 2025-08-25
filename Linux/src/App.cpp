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
    mainScale = ImGui_ImplSDL2_GetContentScaleForDisplay(0);
    windowFlags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    mainWindow = SDL_CreateWindow("Praktyka letnia WETI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)(1280 * mainScale), (int)(720 * mainScale), windowFlags);
    if (mainWindow == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }
    renderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
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

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup scaling
    style = &ImGui::GetStyle();
    style->ScaleAllSizes(mainScale); // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style->FontScaleDpi = mainScale; // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(mainWindow, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    return 0;
}

// Load Fonts
// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
// - Read 'docs/FONTS.md' for more instructions and details.
// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
// style.FontSizeBase = 20.0f;
// io.Fonts->AddFontDefault();
// io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf");
// io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf");
// io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf");
// io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf");
// ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf");
// IM_ASSERT(font != nullptr);

int App::MainLoop()
{

    // Our state

    // tmp
    surface = SDL_LoadBMP("./Fish.bmp");
    tx = SDL_CreateTextureFromSurface(renderer, surface);
    texW = surface->w;
    texH = surface->h;

    SDL_LockSurface(surface);

    uint8_t *surfacePixels = (uint8_t *)surface->pixels;
    for (int i = 0; i < texW; i++)
    {
        for (int j = 0; j < texH; j++)
        {
            // BGR
            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel] = -surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel];
            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 1] = -surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 1];
            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 2] = -surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 2];
        }
    }
    // SDL_memset(surface->pixels, 255, surface->h * surface->pitch);
    SDL_UnlockSurface(surface);

    txO = SDL_CreateTextureFromSurface(renderer, surface);

    float arr[] = {1.0, 2.0, 3.0, 4.0, 5.0};

    while (runLoop)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.

        int eventsCode = HandleEvents();
        if (eventsCode == -2)
            continue;
        if (eventsCode == -1)
            break;

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        DrawMenuBar();
        DrawAlgorihmsBar();

        DrawPictureSpace();
        DrawHistogramsAndFunctions(arr, IM_ARRAYSIZE(arr));

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            // set the pos elewhere, the size is fixed thou

            ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        Render();
    }

    return 0;
}

void App::Cleanup()
{
    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(tx);
    SDL_DestroyTexture(txO);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(mainWindow);
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
        ImGui::MenuItem("Zapisz");
        ImGui::MenuItem("Zapisz jako");
        ImGui::MenuItem("Wczytaj");
        ImGui::Separator();
        ImGui::MenuItem("Wyjdz bez zapisu");
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Ustawienia"))
    {
        ImGui::MenuItem("Motyw");
        ImGui::Separator();
        ImGui::MenuItem("Skroty");
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Pomoc"))
    {
        ImGui::MenuItem("O programie");
        ImGui::Separator();
        ImGui::MenuItem("Skroty klawiszowe");
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}

void App::DrawAlgorihmsBar()
{
    ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight()));
    SDL_GetWindowSize(mainWindow, &currWidth, &currHeight);
    ImGui::SetNextWindowSize(ImVec2(currWidth, ALG_BAR_H));
    ImGui::Begin("Algorytmy", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    if (ImGui::Button("Algorytm1"))
    {
        showAl1 = !showAl1;
    }
    ImGui::SameLine();
    if (ImGui::Button("Algorytm2"))
    {
    }
    ImGui::SameLine();
    if (ImGui::Button("Algorytm3"))
    {
    }
    ImGui::End();

    if (showAl1)
    {
        ImGui::SetNextWindowSize(ImVec2(POPUP_SIZE, POPUP_SIZE));
        ImGui::Begin("Parametry");
        ImGui::End();
    }
}

void App::DrawPictureSpace()
{
    float h = ImGui::GetFrameHeight() + ALG_BAR_H;
    ImGui::SetNextWindowPos(ImVec2(0, h));
    ImGui::SetNextWindowSize(ImVec2(currWidth / 2, currHeight - MENU_ALG_HIST_H));
    ImGui::Begin("Obraz wejsciowy", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize);
    ImGui::SameLine((ImGui::GetWindowWidth() - texW) / 2);
    ImGui::SetCursorPosY((ImGui::GetWindowHeight() - texH) / 2);
    ImGui::Image((ImTextureRef)tx, ImVec2(texW, texH));
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(currWidth / 2, h));
    ImGui::SetNextWindowSize(ImVec2(currWidth / 2, currHeight - MENU_ALG_HIST_H));
    ImGui::Begin("Obraz wyjsciowy", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize);
    ImGui::SameLine((ImGui::GetWindowWidth() - texW) / 2);
    ImGui::SetCursorPosY((ImGui::GetWindowHeight() - texH) / 2);
    ImGui::Image((ImTextureRef)txO, ImVec2(texW, texH));
    ImGui::End();
}

void App::DrawHistogramsAndFunctions(float arr[], int vc)
{
    float h = ImGui::GetFrameHeight() + ALG_BAR_H + (currHeight - MENU_ALG_HIST_H);

    ImGui::SetNextWindowPos(ImVec2(0, h));
    ImGui::SetNextWindowSize(ImVec2(currWidth, HIST_BAR_HEIGHT));
    ImGui::Begin("Histogramy", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    float freeSpace = currWidth - 3 * HIST_WINDOW_W;
    freeSpace -= 2 * BORDER_OFFSET;

    ImGui::SameLine(BORDER_OFFSET);
    ImGui::BeginChild("Histogram wejsciowy", ImVec2(HIST_WINDOW_W, HIST_WINDOW_H), ImGuiChildFlags_Borders);
    ImGui::Text("Historgram wejsciowy");
    ImGui::PlotHistogram("##", arr, vc, 0, NULL, 0.0f, 10.0f, ImVec2(HIST_W, HIST_H));
    ImGui::EndChild();
    ImGui::SameLine(HIST_WINDOW_W + BORDER_OFFSET + freeSpace / 2);
    ImGui::BeginChild("Funkcja transformacji", ImVec2(HIST_WINDOW_W, HIST_WINDOW_H), ImGuiChildFlags_Borders);
    ImGui::Text("Funkcja");
    ImGui::PlotLines("##", arr, vc, 0, NULL, 0.0f, 10.0f, ImVec2(HIST_W, HIST_H));
    ImGui::EndChild();
    ImGui::SameLine(HIST_WINDOW_W * 2 + BORDER_OFFSET + freeSpace);
    ImGui::BeginChild("Histogram wyjsciowy", ImVec2(HIST_WINDOW_W, HIST_WINDOW_H), ImGuiChildFlags_Borders);
    ImGui::Text("Historgram wyjsciowy");
    ImGui::PlotHistogram("##", arr, vc, 0, NULL, 0.0f, 10.0f, ImVec2(HIST_W, HIST_H));
    ImGui::EndChild();
    ImGui::End();
}

void App::Render()
{
    ImGui::Render();
    SDL_RenderSetScale(renderer, io->DisplayFramebufferScale.x, io->DisplayFramebufferScale.y);
    SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
    SDL_RenderClear(renderer);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderPresent(renderer);
}