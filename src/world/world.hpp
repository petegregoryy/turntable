#pragma once
#include "raylib.h"
#include "raymath.h"
#include <cstdint>

constexpr int   MAP_W = 64;
constexpr int   MAP_H = 64;
constexpr float TILE  = 1.0f;   // tile size in world units
constexpr float STEP  = 0.25f;  // vertical metres per height level

enum Dir : uint8_t {
    N  = 1<<0, E  = 1<<1, S  = 1<<2, W  = 1<<3,
    NE = 1<<4, NW = 1<<5, SE = 1<<6, SW = 1<<7
};

struct Tile {
    int     h    = 0;      // height level
    bool    has  = false;  // has track present
    uint8_t mask = 0;      // connection mask if track present
    Color   col  = GREEN;  // surface colour
};

extern Tile gWorld[MAP_W][MAP_H];

Vector3 toWorld(int x,int y,float yOff=0.0f);
bool tileHasTrack(int x,int y);
void recalcMask(int x,int y);
void toggleTrack(int x,int y);
void modifyHeight(int x,int y,int delta);
void generateHills();
