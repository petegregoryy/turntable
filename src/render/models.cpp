#include "models.hpp"
#include "../world/world.hpp"

Model gEW, gNS, gDG;

static Model makeCube(float w,float h,float l, Color tint)
{
    Mesh m = GenMeshCube(w,h,l);
    Model mdl = LoadModelFromMesh(m);
    mdl.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = tint;
    return mdl;
}

void initModels()
{
    gEW = makeCube(TILE*0.9f, 0.1f, TILE*0.2f, GRAY);
    gNS = makeCube(TILE*0.2f, 0.1f, TILE*0.9f, GRAY);
    gDG = makeCube(TILE*1.0f, 0.1f, TILE*0.2f, GRAY); // rotate 45° when drawn
}
