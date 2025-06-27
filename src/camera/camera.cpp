#include "camera.hpp"
#include "raylib.h"
#include "raymath.h"

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
    const float s = 0.70710678f; // 1/sqrt(2)
    Vector3 right   = { s,0,-s };
    Vector3 forward = { s,0, s };
    Vector3 move = Vector3Add(Vector3Scale(right, dx), Vector3Scale(forward, dz));
    cam.target = Vector3Add(cam.target, move);
    update();
}

void IsoCam::zoom(float wheel) {
    dist = Clamp(dist - wheel*5.0f, 10.0f, 120.0f);
    update();
}


