#pragma once
#include "../camera/camera.hpp"

void drawGround();
void drawTracks();
void drawGrid();
bool pickTile(const IsoCam& c, int& gx, int& gy);
