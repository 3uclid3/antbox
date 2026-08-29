#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

auto main() -> int
{
    // Initialization
    int screenWidth = 1280;
    int screenHeight = 800;

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "antbox - an ant-colony simulation sandbox");
    SetTargetFPS(144);
    rlImGuiSetup(true);

    // Texture image = LoadTexture("resources/parrots.png");

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        // start ImGui Content
        rlImGuiBegin();

        // show ImGui Content
        bool open = true;
        ImGui::ShowDemoWindow(&open);

        open = true;
        if (ImGui::Begin("Test Window", &open))
        {
            ImGui::TextUnformatted(ICON_FA_JEDI);

            // rlImGuiImage(&image);
        }
        ImGui::End();

        // end ImGui Content
        rlImGuiEnd();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            DrawText("Pressed", 0, 0, 20, RED);

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            DrawText("Down", 0, 20, 20, GREEN);

        if (IsWindowFocused())
            DrawText("Focused", 100, 20, 20, WHITE);

        EndDrawing();
    }

    // De-Initialization
    rlImGuiShutdown();
    // UnloadTexture(image);
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
