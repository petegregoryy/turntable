#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include <cmath>

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

    auto determineMask = [](int gx,int gy,const Vector3& pos) {
        Vector3 c = toWorld(gx, gy);
        float dx = (pos.x - c.x) / TILE;
        float dz = (pos.z - c.z) / TILE;
        float dEW   = fabsf(dz);
        float dNS   = fabsf(dx);
        float dNESW = fabsf(dx - dz) * 0.7071f;
        float dNWSE = fabsf(dx + dz) * 0.7071f;
        float best  = dEW; uint8_t mask = E|W;
        if(dNS < best)  { best = dNS;  mask = N|S; }
        if(dNESW < best){ best = dNESW; mask = NE|SW; }
        if(dNWSE < best){ best = dNWSE; mask = NW|SE; }
        return mask;
    };

    while(!WindowShouldClose()) {
        float dt = GetFrameTime();

        float pan = 15.0f * dt;
        if (IsKeyDown(KEY_A)) cam.pan(-pan, 0);
        if (IsKeyDown(KEY_D)) cam.pan( pan, 0);
        if (IsKeyDown(KEY_W)) cam.pan(0, -pan);
        if (IsKeyDown(KEY_S)) cam.pan(0,  pan);
        cam.zoom(GetMouseWheelMove());

        int gx, gy; Vector3 hitPos;
        bool hovered = pickTile(cam, gx, gy, &hitPos);
        if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (IsKeyDown(KEY_LEFT_SHIFT))       modifyHeight(gx, gy, +1);
            else if (IsKeyDown(KEY_LEFT_CONTROL)) modifyHeight(gx, gy, -1);
            else                                  placeTrack(gx, gy, determineMask(gx,gy,hitPos));
        }

        BeginDrawing();
        ClearBackground(SKYBLUE);
        BeginMode3D(cam.cam);
        drawGround();
        drawTracks();
        if(hovered) drawGhostTrack(gx, gy, determineMask(gx,gy,hitPos));
        drawGrid();
        EndMode3D();
        DrawText("LMB place track | Shift+LMB raise | Ctrl+LMB lower | WASD pan | wheel zoom",
                 10, 10, 16, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
