#include "world.hpp"
#include <cmath>

Tile gWorld[MAP_W][MAP_H];
int  gHeights[MAP_W+1][MAP_H+1];

static float heightAt(int gx,int gy) {
    gx = Clamp(gx,0,MAP_W);
    gy = Clamp(gy,0,MAP_H);
    return gHeights[gx][gy] * STEP;
}

float getTileHeight(int x,int y) {
    return (heightAt(x, y) + heightAt(x+1,y) +
            heightAt(x,y+1) + heightAt(x+1,y+1)) * 0.25f;
}

Vector3 cornerPos(int x,int y,float yOff)
{
    return { (x - MAP_W/2) * TILE, heightAt(x,y)+yOff, (y - MAP_H/2) * TILE };
}

Vector3 toWorld(int x,int y,float yOff)
{
    return { (x + 0.5f - MAP_W/2) * TILE,
             getTileHeight(x,y) + yOff,
             (y + 0.5f - MAP_H/2) * TILE };
}

void placeTrack(int x,int y,uint8_t mask)
{
    if(x<0||x>=MAP_W||y<0||y>=MAP_H) return;
    if(gWorld[x][y].has && gWorld[x][y].mask == mask) {
        gWorld[x][y].has  = false;
        gWorld[x][y].mask = 0;
    } else {
        gWorld[x][y].has  = true;
        gWorld[x][y].mask = mask;
    }
}

void modifyHeight(int x,int y,int delta)
{
    if(x<0||x>MAP_W||y<0||y>MAP_H) return;
    int* verts[4] = { &gHeights[x][y], &gHeights[x+1][y],
                      &gHeights[x][y+1], &gHeights[x+1][y+1] };
    for(int i=0;i<4;++i)
        *verts[i] = Clamp(*verts[i] + delta, -16, 32);
}

void generateHills()
{
    Color ground = {0, 120, 0, 255};
    for(int y=0; y<=MAP_H; ++y)
        for(int x=0; x<=MAP_W; ++x)
            gHeights[x][y] = 0;

    for(int y=0; y<MAP_H; ++y)
        for(int x=0; x<MAP_W; ++x)
            gWorld[x][y].col = ground;
}
