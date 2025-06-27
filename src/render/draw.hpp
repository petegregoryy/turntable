#pragma once
#include "../camera/camera.hpp"
#include <cstdint>
#include "raylib.h"

void drawGround();
void drawTracks();
void drawGhostTrack(int x,int y,uint8_t mask);
void drawGrid();
bool pickTile(const IsoCam& c, int& gx, int& gy, Vector3* pos = nullptr);
