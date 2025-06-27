#include "world.hpp"
#include <cmath>

Tile gWorld[MAP_W][MAP_H];

Vector3 toWorld(int x,int y,float yOff)
{
    return { (x - MAP_W/2) * TILE, yOff, (y - MAP_H/2) * TILE };
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
    if(x<0||x>=MAP_W||y<0||y>=MAP_H) return;
    gWorld[x][y].h = Clamp(gWorld[x][y].h + delta, -4, 8);
}

void generateHills()
{
    Color ground = {0, 120, 0, 255};
    for(int y=0; y<MAP_H; ++y)
        for(int x=0; x<MAP_W; ++x) {
            gWorld[x][y].h   = 0;
            gWorld[x][y].col = ground;
        }
}
