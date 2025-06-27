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
            Vector3 v00 = cornerPos(x,   y);
            Vector3 v10 = cornerPos(x+1, y);
            Vector3 v01 = cornerPos(x,   y+1);
            Vector3 v11 = cornerPos(x+1, y+1);
            Color c = gWorld[x][y].col;
            DrawTriangle3D(v00, v10, v11, c);
            DrawTriangle3D(v00, v11, v01, c);
        }
}

void drawTracks()
{
    for(int y=0; y<MAP_H; ++y)
        for(int x=0; x<MAP_W; ++x) {
            if(!gWorld[x][y].has) continue;
            uint8_t m = gWorld[x][y].mask;
            if(!m) continue;
            Vector3 base = toWorld(x, y, 0.05f);
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

void drawGhostTrack(int x,int y,uint8_t m)
{
    if(!m) return;
    Vector3 base = toWorld(x, y, 0.05f);
    Color c = {200,200,200,120};
    if(m == (E|W)) DrawModel(gEW, base, 1.0f, c);
    if(m == (N|S)) DrawModel(gNS, base, 1.0f, c);
    if(m == (NE|SW)) {
        rlPushMatrix();
        rlTranslatef(base.x, base.y, base.z);
        rlRotatef(45,0,1,0);
        DrawModel(gDG,{0,0,0},1.0f,c);
        rlPopMatrix();
    }
    if(m == (NW|SE)) {
        rlPushMatrix();
        rlTranslatef(base.x, base.y, base.z);
        rlRotatef(-45,0,1,0);
        DrawModel(gDG,{0,0,0},1.0f,c);
        rlPopMatrix();
    }
}

void drawGrid()
{
    rlDisableDepthTest();
    Color g = {0,80,0,255};
    for(int y=0; y<MAP_H; ++y)
        for(int x=0; x<MAP_W; ++x) {
            Vector3 c = toWorld(x,y,0.02f);
            DrawLine3D({c.x - TILE/2, c.y, c.z}, {c.x + TILE/2, c.y, c.z}, g);
            DrawLine3D({c.x, c.y, c.z - TILE/2}, {c.x, c.y, c.z + TILE/2}, g);
        }
    rlEnableDepthTest();
}

bool pickTile(const IsoCam& c, int& gx, int& gy, Vector3* pos)
{
    Ray ray = GetMouseRay(GetMousePosition(), c.cam);
    RayCollision hit = GetRayCollisionQuad(ray,
        {-10000,0,-10000}, {10000,0,-10000}, {10000,0,10000}, {-10000,0,10000});
    if(!hit.hit) return false;
    gx = int(std::floor(hit.point.x / TILE + MAP_W/2));
    gy = int(std::floor(hit.point.z / TILE + MAP_H/2));
    if(pos) *pos = hit.point;
    return gx>=0 && gx<MAP_W && gy>=0 && gy<MAP_H;
}
