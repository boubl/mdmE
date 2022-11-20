#include <stdio.h>
#include <SDL2/SDL.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdlrenderer.h>
#include <imgui/backends/imgui_impl_sdl.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "bms.h"
#include "ui.h"
#include "bass/bass.h"
#include "IconsFontAwesome5.h"
#include "editor.h"
#include "data.h"
#include "locales.h"
#include "audio.h"
#include <discord/discord.h>
#include "drpc.h"

// Main code
int main(int, char**)
{
    setlocale(LC_ALL, "LC_CTYPE=.utf8");
#pragma region init sdl and imgui

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    if (TTF_Init() < 0) {
        cout << "Error initializing SDL_ttf: " << TTF_GetError() << endl;
    }
    
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        SDL_Log("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }

    // Discord RPC init
    if (DRpc::Init()) {
        std::cout << "Failed to instantiate discord core! Is Discord's destkop app open?\n";
    }
    DRpc::ChangeChartname("Nothing");

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
    SDL_Window* window = SDL_CreateWindow("mdmE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImFont* defaultFont = io.Fonts->AddFontFromFileTTF("data/fonts/Noto.ttf", 20.0f);

    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphMinAdvanceX = 16.0f; // make the icons monospaced
    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    io.Fonts->AddFontFromFileTTF("data/fonts/fa-solid-900.ttf", 13.0f, &config, icon_ranges);
    io.Fonts->AddFontFromFileTTF("data/fonts/NotoTC.ttf", 16.0f, &config, io.Fonts->GetGlyphRangesChineseFull());
    io.Fonts->AddFontFromFileTTF("data/fonts/NotoJP.ttf", 16.0f, &config, io.Fonts->GetGlyphRangesJapanese());
    io.Fonts->AddFontFromFileTTF("data/fonts/NotoKR.ttf", 16.0f, &config, io.Fonts->GetGlyphRangesKorean());

    ImFont* monoFont = io.Fonts->AddFontFromFileTTF("data/fonts/NotoMono.ttf", 20.0f);
    io.Fonts->Build();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.00f);

    BASS_Init(-1, 44100, BASS_DEVICE_STEREO, 0, NULL);
    cout << "Bass init code: " << BASS_ErrorGetCode() << endl;
    BASS_Start();

#pragma endregion

    Data::InitFonts(defaultFont, monoFont);
    Data::LoadKits();

    BMS::MDMFile file = BMS::MDMFile();
    Data::file = &file;

    ui::MainLayout layout = ui::MainLayout(&file);

    Audio::Init();
    Audio::ReloadSamples();
    Editor::Init(renderer);
    Localization loc;
    loc.LoadStrings();

    // Main loop
    bool done = false;
    while (!done)
    {
        DRpc::RunCallbacks();
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == (Uint32)SDL_QUIT)
                done = true;
            if (event.type == (Uint32)SDL_WINDOWEVENT && event.window.event == (Uint32)SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        layout.DrawUI();

        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        
        SDL_RenderClear(renderer);
        // Rendering

        Editor::DisplayChannels();
        Audio::PlaySFXs();

        // ImGui should be renderer in last, other rendering should be on top ^^
        ImGui::Render();
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    BASS_Free();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
