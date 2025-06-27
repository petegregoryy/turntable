#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include <cstdint>
#include <cmath>

// ─── Config ────────────────────────────────────────────
constexpr int   WIN_W = 1280;
constexpr int   WIN_H = 720;
constexpr int   MAP_W = 64;
constexpr int   MAP_H = 64;
constexpr float TILE  = 1.0f;   // tile size in world units
constexpr float STEP  = 0.25f;  // vertical metres per height level

// Track connection bit‑flags
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
static Tile gWorld[MAP_W][MAP_H];

// ─── Models ────────────────────────────────────────────
static Model gEW, gNS, gDG;   // track models (east‑west, north‑south, diag)

static Model makeCube(float w,float h,float l, Color tint)
{
    Mesh m = GenMeshCube(w,h,l);
    Model mdl = LoadModelFromMesh(m);
    mdl.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = tint;
    return mdl;
}

static void initModels()
{
    gEW = makeCube(TILE*0.9f, 0.1f, TILE*0.2f, GRAY);
    gNS = makeCube(TILE*0.2f, 0.1f, TILE*0.9f, GRAY);
    gDG = makeCube(TILE*1.0f, 0.1f, TILE*0.2f, GRAY); // rotate 45° when drawn
}

// ─── Helpers ───────────────────────────────────────────
static Vector3 toWorld(int x,int y,float yOff=0.0f)
{
    return { (x - MAP_W/2) * TILE, yOff, (y - MAP_H/2) * TILE };
}

static bool tileHasTrack(int x,int y)
{
    return x>=0 && x<MAP_W && y>=0 && y<MAP_H && gWorld[x][y].has;
}

static void recalcMask(int x,int y)
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

static void toggleTrack(int x,int y)
{
    if(x<0||x>=MAP_W||y<0||y>=MAP_H) return;
    gWorld[x][y].has = !gWorld[x][y].has;
    for(int dy=-1; dy<=1; ++dy)
        for(int dx=-1; dx<=1; ++dx)
            recalcMask(x+dx, y+dy);
}

static void modifyHeight(int x,int y,int delta)
{
    if(x<0||x>=MAP_W||y<0||y>=MAP_H) return;
    gWorld[x][y].h = Clamp(gWorld[x][y].h + delta, -4, 8);
}

static void generateHills()
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

// ─── Camera ───────────────────────────────────────────
struct IsoCam {
    Camera3D cam{};
    float dist = 35.0f;
    IsoCam() {
        cam.target = {0,0,0};
        cam.up     = {0,1,0};
        cam.fovy   = 45;
        cam.projection = CAMERA_PERSPECTIVE;
        update();
    }
    void update() {
        cam.position = { cam.target.x + dist, cam.target.y + dist, cam.target.z + dist };
    }
    void pan(float dx,float dz) {
        cam.target = Vector3Add(cam.target, {dx,0,dz});
        update();
    }
    void zoom(float wheel) {
        dist = Clamp(dist - wheel*5.0f, 10.0f, 120.0f);
        update();
    }
};

// ─── Drawing ───────────────────────────────────────────
static void drawGround()
{
    for(int y=0; y<MAP_H; ++y)
        for(int x=0; x<MAP_W; ++x) {
            float   h   = gWorld[x][y].h * STEP;
            Vector3 pos = toWorld(x, y, h/2.0f);
            DrawCube(pos, TILE, h + 0.05f, TILE, gWorld[x][y].col);
        }
}

static void drawTracks()
{
    for(int y=0; y<MAP_H; ++y)
        for(int x=0; x<MAP_W; ++x) {
            if(!gWorld[x][y].has) continue;
            uint8_t m = gWorld[x][y].mask;
            if(!m) continue;
            Vector3 base = toWorld(x, y, gWorld[x][y].h*STEP + 0.05f);
            if(m & (E|W)) DrawModel(gEW, base, 1.0f, WHITE);
            if(m & (N|S)) DrawModel(gNS, base, 1.0f, WHITE);
            if(m & (NE|SW)) {
                rlPushMatrix();
                rlTranslatef(base.x, base.y, base.z);
                rlRotatef(45, 0,1,0);
                DrawModel(gDG, {0,0,0}, 1.0f, WHITE);
                rlPopMatrix();
            }
            if(m & (NW|SE)) {
                rlPushMatrix();
                rlTranslatef(base.x, base.y, base.z);
                rlRotatef(-45, 0,1,0);
                DrawModel(gDG, {0,0,0}, 1.0f, WHITE);
                rlPopMatrix();
            }
        }
}

static void drawGrid()
{
    rlDisableDepthTest();
    Color g = {255,255,255,30};
    for(int y=0; y<=MAP_H; ++y)
        DrawLine3D(toWorld(0,y,0.02f), toWorld(MAP_W,y,0.02f), g);
    for(int x=0; x<=MAP_W; ++x)
        DrawLine3D(toWorld(x,0,0.02f), toWorld(x,MAP_H,0.02f), g);
    rlEnableDepthTest();
}

// ─── Picking ───────────────────────────────────────────
static bool pickTile(const IsoCam& c, int& gx, int& gy)
{
    Ray ray = GetMouseRay(GetMousePosition(), c.cam);
    RayCollision hit = GetRayCollisionQuad(ray,
        {-10000,0,-10000}, {10000,0,-10000}, {10000,0,10000}, {-10000,0,10000});
    if(!hit.hit) return false;
    gx = int(std::floor(hit.point.x / TILE + MAP_W/2));
    gy = int(std::floor(hit.point.z / TILE + MAP_H/2));
    return gx>=0 && gx<MAP_W && gy>=0 && gy<MAP_H;
}

// ─── Main ─────────────────────────────────────────────
int main()
{
    InitWindow(WIN_W, WIN_H, "Turntable Prototype");
    SetTargetFPS(60);
    initModels();
    generateHills();
    IsoCam cam;

    while(!WindowShouldClose()) {
        float dt = GetFrameTime();

        // Camera movement
        float pan = 15.0f * dt;
        if (IsKeyDown(KEY_A)) cam.pan( pan, 0);
        if (IsKeyDown(KEY_D)) cam.pan(-pan, 0);
        if (IsKeyDown(KEY_W)) cam.pan(0,  pan);
        if (IsKeyDown(KEY_S)) cam.pan(0, -pan);
        cam.zoom(GetMouseWheelMove());

        // Mouse editing
        int gx, gy;
        if (pickTile(cam, gx, gy) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (IsKeyDown(KEY_LEFT_SHIFT))       modifyHeight(gx, gy, +1);
            else if (IsKeyDown(KEY_LEFT_CONTROL)) modifyHeight(gx, gy, -1);
            else                                  toggleTrack(gx, gy);
        }

        BeginDrawing();
        ClearBackground(SKYBLUE);
        BeginMode3D(cam.cam);
        drawGround();
        drawTracks();
        drawGrid();
        EndMode3D();
        DrawText("LMB toggle track | Shift+LMB raise | Ctrl+LMB lower | WASD pan | wheel zoom",
                 10, 10, 16, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

