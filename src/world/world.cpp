#include "world.hpp"
#include <cmath>

Tile gWorld[MAP_W][MAP_H];

Vector3 toWorld(int x,int y,float yOff)
{
    return { (x - MAP_W/2) * TILE, yOff, (y - MAP_H/2) * TILE };
}

bool tileHasTrack(int x,int y)
{
    return x>=0 && x<MAP_W && y>=0 && y<MAP_H && gWorld[x][y].has;
}

void recalcMask(int x,int y)
{
    if(x<0||x>=MAP_W||y<0||y>=MAP_H) return;
    if(!gWorld[x][y].has) { gWorld[x][y].mask = 0; return; }
    uint8_t m = 0;
    if(tileHasTrack(x,   y-1)) m |= N;
    if(tileHasTrack(x+1, y  )) m |= E;
    if(tileHasTrack(x,   y+1)) m |= S;
    if(tileHasTrack(x-1, y  )) m |= W;
    if(tileHasTrack(x+1, y-1)) m |= NE;
    if(tileHasTrack(x-1, y-1)) m |= NW;
    if(tileHasTrack(x+1, y+1)) m |= SE;
    if(tileHasTrack(x-1, y+1)) m |= SW;
    gWorld[x][y].mask = m;
}

void toggleTrack(int x,int y)
{
    if(x<0||x>=MAP_W||y<0||y>=MAP_H) return;
    gWorld[x][y].has = !gWorld[x][y].has;
    for(int dy=-1; dy<=1; ++dy)
        for(int dx=-1; dx<=1; ++dx)
            recalcMask(x+dx, y+dy);
}

void modifyHeight(int x,int y,int delta)
{
    if(x<0||x>=MAP_W||y<0||y>=MAP_H) return;
    gWorld[x][y].h = Clamp(gWorld[x][y].h + delta, -4, 8);
}

void generateHills()
{
    for(int y=0; y<MAP_H; ++y)
        for(int x=0; x<MAP_W; ++x) {
            float fx = float(x)/MAP_W * 6.283f;
            float fy = float(y)/MAP_H * 6.283f;
            int   h  = int(std::round(std::sin(fx)*std::sin(fy) * 3.0f));
            gWorld[x][y].h   = h;
            gWorld[x][y].col = (h < 0 ? BLUE : GREEN);
        }
}
