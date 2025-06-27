#include "models.hpp"
#include "../world/world.hpp"

Model gEW, gNS, gDG;

static Mesh makeDiagMesh(float w,float h)
{
    float half = TILE/2.0f;
    float hw   = w/2.0f;
    float hh   = h/2.0f;

    Vector3 v[8] = {
        { -half + hw, -hh, -half },      // 0
        { -half,      -hh, -half + hw }, // 1
        {  half,      -hh,  half - hw }, // 2
        {  half - hw, -hh,  half },      // 3
        { -half + hw,  hh, -half },      // 4
        { -half,       hh, -half + hw }, // 5
        {  half,       hh,  half - hw }, // 6
        {  half - hw,  hh,  half }       // 7
    };

    unsigned short ind[36] = {
        0,1,2, 2,1,3,       // bottom
        4,6,5, 6,7,5,       // top
        0,4,5, 0,5,1,
        1,5,7, 1,7,3,
        2,3,7, 2,7,6,
        0,2,6, 0,6,4
    };

    Mesh m{};
    m.vertexCount   = 8;
    m.triangleCount = 12;
    m.vertices  = (float*)MemAlloc(m.vertexCount*3*sizeof(float));
    m.indices   = (unsigned short*)MemAlloc(m.triangleCount*3*sizeof(unsigned short));
    for(int i=0;i<8;++i) {
        m.vertices[i*3+0] = v[i].x;
        m.vertices[i*3+1] = v[i].y;
        m.vertices[i*3+2] = v[i].z;
    }
    for(int i=0;i<36;++i) m.indices[i] = ind[i];
    UploadMesh(&m, false);
    return m;
}

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
    Mesh dm = makeDiagMesh(TILE*0.2f, 0.1f);
    gDG = LoadModelFromMesh(dm);
    gDG.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = GRAY;
}
