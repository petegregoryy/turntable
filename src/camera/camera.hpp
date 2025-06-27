#pragma once
#include "raylib.h"

struct IsoCam {
    Camera3D cam{};
    float dist = 35.0f;
    IsoCam();
    void update();
    void pan(float dx, float dz);
    void zoom(float wheel);
};
