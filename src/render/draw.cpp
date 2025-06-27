#include "draw.hpp"
#include "models.hpp"
#include "../world/world.hpp"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include <cmath>

void drawGround()
{
    for(int y=0; y<MAP_H; ++y)
        for(int x=0; x<MAP_W; ++x) {
            float   h   = gWorld[x][y].h * STEP;
            Vector3 pos = toWorld(x, y, h/2.0f);
            DrawCube(pos, TILE, h + 0.05f, TILE, gWorld[x][y].col);
        }
}

void drawTracks()
{
    for(int y=0; y<MAP_H; ++y)
        for(int x=0; x<MAP_W; ++x) {
            if(!gWorld[x][y].has) continue;
            uint8_t m = gWorld[x][y].mask;
            if(!m) continue;
            Vector3 base = toWorld(x, y, gWorld[x][y].h*STEP + 0.05f);
            if(m & (E|W)) DrawModel(gEW, base, 1.0f, WHITE);
            if(m & (N|S)) DrawModel(gNS, base, 1.0f, WHITE);
            if(m & (NE|SW)) {
                rlPushMatrix();
                rlTranslatef(base.x, base.y, base.z);
                rlRotatef(45, 0,1,0);
                DrawModel(gDG, {0,0,0}, 1.0f, WHITE);
                rlPopMatrix();
            }
            if(m & (NW|SE)) {
                rlPushMatrix();
                rlTranslatef(base.x, base.y, base.z);
                rlRotatef(-45, 0,1,0);
                DrawModel(gDG, {0,0,0}, 1.0f, WHITE);
                rlPopMatrix();
            }
        }
}

void drawGrid()
{
    rlDisableDepthTest();
    Color g = {255,255,255,30};
    for(int y=0; y<=MAP_H; ++y)
        DrawLine3D(toWorld(0,y,0.02f), toWorld(MAP_W,y,0.02f), g);
    for(int x=0; x<=MAP_W; ++x)
        DrawLine3D(toWorld(x,0,0.02f), toWorld(x,MAP_H,0.02f), g);
    rlEnableDepthTest();
}

bool pickTile(const IsoCam& c, int& gx, int& gy)
{
    Ray ray = GetMouseRay(GetMousePosition(), c.cam);
    RayCollision hit = GetRayCollisionQuad(ray,
        {-10000,0,-10000}, {10000,0,-10000}, {10000,0,10000}, {-10000,0,10000});
    if(!hit.hit) return false;
    gx = int(std::floor(hit.point.x / TILE + MAP_W/2));
    gy = int(std::floor(hit.point.z / TILE + MAP_H/2));
    return gx>=0 && gx<MAP_W && gy>=0 && gy<MAP_H;
}
