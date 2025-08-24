#include "common.h"
#include "raygui.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int finishScreen = 0; 

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

void InitEngine(void);
void InitCamera(void);
void InitTheme(const char* theme);

void UpdateMouse(void);
void PollEvents(void);
int CheckTargetHit(void);
int checkInterference(Vector3 pos);

void Gridshot(void);

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    LoadMaps("resources/maps", MAX_ENGINES);
    InitDraw();
    InitTheme("DARK");
    InitEngine();

    DisableCursor();
    SetTargetFPS(300);              
}

void UpdateGameplayScreen(void)
{
    if (menu == MAIN) {UpdateMouse();}
    targetEngine.Update();

    PollEvents();
    UpdateGlobals();
    int target = CheckTargetHit();

}

void DrawGameplayScreen(void)
{
    DrawMap();
    DrawCrosshair();
    DrawGUI();
    
}

void UnloadGameplayScreen(void)
{
    UnloadDraw();
    free(targets);
}

int FinishGameplayScreen(void)
{
    return finishScreen;
}


//----------------------------------------------------------------------------------
// Private Functions
//----------------------------------------------------------------------------------

void InitCamera(void)
{
    gameEngine.sensitivity = 1.0f;
    gameEngine.volume = 1.0f;

    LoadSettings("settings.txt", &gameEngine);

    //CAMERA SETTINGS
    gameEngine.camera.position = (Vector3){ 0.0f, 2.0f, 0.0f };
    gameEngine.camera.target = (Vector3){ 0.0f, 2.0f, 1.0f };
    gameEngine.camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    gameEngine.camera.fovy = 70.53f;
    gameEngine.camera.projection = CAMERA_CUSTOM;
}

void InitEngine(void) 
{
    InitCamera();

    //Initial Engine is 3x3 Gridshot
    targetEngine.mapType = 0;
    targetEngine.targetSize = 0.45f;
    targetEngine.targetCount = 3;
    targetEngine.gap = 10;
    targetEngine.targetHealth = 1;
    targetEngine.minZ = 60;
    targetEngine.maxZ = 50;
    targetEngine.xVar = 30;
    targetEngine.yVar = 30;

    targetEngine = engines[2];   

    targets = malloc(targetEngine.targetCount * sizeof(Target));
    for (int i = 0; i < targetEngine.targetCount; i++)
    {
        Target t = {{0,0,0}, 0};
        targets[i] = t;
    }

    switch (targetEngine.mapType) 
    {
        case (GRIDSHOT):  targetEngine.Update = Gridshot; break;
        case (TRACK): targetEngine.Update = Track; break;
    }
}

void PollEvents(void)
{
    if (IsKeyPressed(KEY_TAB))
    {
        menu = (menu == PAUSE || menu == SETTINGS) ? MAIN : PAUSE;

        if (menu == PAUSE) {EnableCursor();}
        else {DisableCursor();}

    }

}

int CheckTargetHit(void)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && menu == MAIN)
    {
        int n = targetEngine.targetCount;
        for (int i = 0; i < n; i++)
        {
            RayCollision sphereCollision = GetRayCollisionSphere(gameEngine.mouseRay, targets[i].position, targetEngine.targetSize);

            if (sphereCollision.hit)
            {

                PlaySound(fxCoin);
                targets[i].health -= 1;
                return i; // Return the index of the hit sphere

            }
        }
    }
    return -1;
}




// Global camera angles
float yaw = 0.0f;
float pitch = 0.0f;

// Call this every frame
void UpdateMouse()
{
    Vector2 delta = GetMouseDelta();  

    int mouseDPI = 800; 

    const float baseDPI = 800.0f;    
    const float degPerCount = 0.07f; 

    float dpiScale = mouseDPI / baseDPI;

    float dx = delta.x * gameEngine.sensitivity * degPerCount * dpiScale * DEG2RAD;
    float dy = delta.y *  gameEngine.sensitivity * degPerCount * dpiScale * DEG2RAD;

    yaw   -= dx;   
    pitch += -dy;  

    float pitchLimit = 89.0f * DEG2RAD;
    if (pitch > pitchLimit) pitch = pitchLimit;
    if (pitch < -pitchLimit) pitch = -pitchLimit;

    Vector3 forward;
    forward.x = cosf(pitch) * sinf(yaw);
    forward.y = sinf(pitch);
    forward.z = cosf(pitch) * cosf(yaw);

    gameEngine.camera.target = Vector3Add(gameEngine.camera.position, forward);
    Vector2 screenCenter = { GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    gameEngine.mouseRay = GetMouseRay(screenCenter, gameEngine.camera);
}


