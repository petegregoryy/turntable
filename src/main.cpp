#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include "world/world.hpp"
#include "render/models.hpp"
#include "render/draw.hpp"
#include "camera/camera.hpp"

constexpr int WIN_W = 1280;
constexpr int WIN_H = 720;

int main()
{
    InitWindow(WIN_W, WIN_H, "Turntable Prototype");
    SetTargetFPS(60);
    initModels();
    generateHills();
    IsoCam cam;

    while(!WindowShouldClose()) {
        float dt = GetFrameTime();

        float pan = 15.0f * dt;
        if (IsKeyDown(KEY_A)) cam.pan( pan, 0);
        if (IsKeyDown(KEY_D)) cam.pan(-pan, 0);
        if (IsKeyDown(KEY_W)) cam.pan(0,  pan);
        if (IsKeyDown(KEY_S)) cam.pan(0, -pan);
        cam.zoom(GetMouseWheelMove());

        int gx, gy;
        if (pickTile(cam, gx, gy) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (IsKeyDown(KEY_LEFT_SHIFT))       modifyHeight(gx, gy, +1);
            else if (IsKeyDown(KEY_LEFT_CONTROL)) modifyHeight(gx, gy, -1);
            else                                  toggleTrack(gx, gy);
        }

        BeginDrawing();
        ClearBackground(SKYBLUE);
        BeginMode3D(cam.cam);
        drawGround();
        drawTracks();
        drawGrid();
        EndMode3D();
        DrawText("LMB toggle track | Shift+LMB raise | Ctrl+LMB lower | WASD pan | wheel zoom",
                 10, 10, 16, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
