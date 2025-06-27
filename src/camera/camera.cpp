#include "../camera/camera.hpp"
#include "raylib.h"

IsoCam::IsoCam() {
    cam.target = {0,0,0};
    cam.up     = {0,1,0};
    cam.fovy   = 45;
    cam.projection = CAMERA_PERSPECTIVE;
    update();
}

void IsoCam::update() {
    cam.position = { cam.target.x + dist, cam.target.y + dist, cam.target.z + dist };
}

void IsoCam::pan(float dx, float dz) {
    cam.target = Vector3Add(cam.target, {dx,0,dz});
    update();
}

void IsoCam::zoom(float wheel) {
    dist = Clamp(dist - wheel*5.0f, 10.0f, 120.0f);
    update();
}


