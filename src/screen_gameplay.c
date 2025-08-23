#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "rcamera.h"
#include "screens.h"
#include "assert.h"
#include "rlgl.h"
#include "raymath.h"
#include "raygui.h"
#include "math.h"

//----------------------------------------------------------------------------------
// Structures
//----------------------------------------------------------------------------------

typedef struct target {
    Vector3 position;
    int health;
} Target;

typedef struct game_engine {
    float time;
    float sphereSize;
    int targetCount;
    int targetHealth;

    int gap;
    int xVar;
    int yVar;

    int minZ;
    int maxZ;

    void (*Update)(void);
} GameEngine;

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
enum MENU { MAIN, PAUSE, SETTINGS };
static int menu = MAIN;
static int centerX = {0};
static int centerY = {0};

static int finishScreen = 0; 
static Camera camera = { 0 };
Model skybox = { 0 };
static Ray mouseRay = { 0 };
static GameEngine gameEngine = { 0 };

//----------------------------------------------------------------------------------
// Game Variables
//----------------------------------------------------------------------------------
static float sensitivity = 1.0f;
static float volume = 1.0f;

static Color targetColors[] = { {210, 210, 0, 255}, RED, GREEN, BLUE, YELLOW, ORANGE, PURPLE };
static const char *colorNames[] = {"Dark Yellow", "Red", "Green", "Blue", "Yellow", "Orange", "Purple" };
static int colorIndex = 0;
static int hudColorIndex = 2;

 static Target* targets;

static float targetSize = 0.5f;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------
void DrawCrosshair(void);
void DrawMap(void);
void DrawGUI(void);
void UpdateMouse(void);
void PollEvents(void);
void InitCamera(void);
void DrawTargets(void);
int CheckTargetHit(void);
void DrawPauseMenu(void);
void DrawSettingsMenu(void);
void DrawHUD(void);
void InitTheme(const char* theme);
void UpdateGlobals(void);

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    //GLOBAL SETTINGS

    // ToggleFullscreen();
    InitCamera();
    InitTheme("DARK");

    //SKYBOX
    skybox = LoadModel("resources/models/skybox.glb");
    assert(skybox.meshCount > 0);

    //GAME START
    DisableCursor();
    SetTargetFPS(300);              
}

void UpdateGameplayScreen(void)
{
    if (menu == MAIN) {UpdateMouse();}
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
    UnloadModel(skybox);
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
     //CAMERA SETTINGS
    camera.position = (Vector3){ 0.0f, 2.0f, 0.0f };
    camera.target = (Vector3){ 0.0f, 2.0f, 1.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 70.53f;
    camera.projection = CAMERA_CUSTOM;
}

void DrawCrosshair(void)
{
    int centerX = GetScreenWidth() / 2;
    int centerY = GetScreenHeight() / 2;
    int size = 6;  // size of the crosshair arms

    int thickness = 2;
    DrawRectangle(centerX - size, centerY - thickness / 2, size * 2, thickness, RAYWHITE); // horizontal bar
    DrawRectangle(centerX - thickness / 2, centerY - size, thickness, size * 2, RAYWHITE); // vertical bar
}

void DrawMap(void)
{
    BeginMode3D(camera);

    //SKYBOX!!!
    rlDisableBackfaceCulling();
    rlDisableDepthMask();
    DrawModel(skybox, (Vector3){0, 0, 0}, 1.0f, WHITE);
    rlEnableDepthMask();
    rlEnableBackfaceCulling();

    //FLOOR
    DrawGrid(4, 0.5f);

    //SPHERES
    DrawTargets();

    EndMode3D();
}

void DrawGUI(void)
{

    DrawHUD();
    if (menu == PAUSE)  { DrawPauseMenu(); }
    else if (menu == SETTINGS) { DrawSettingsMenu(); }

}

void DrawHUD(void)
{
    int padding = 30;
    int gap = 45;

    int spacing = 40;

    //Fade Overlay
    DrawRectangle(0, 0, 235, 155, (Color){ 0, 0, 0, 150 });

    DrawText("Time:", padding, padding, 30, targetColors[hudColorIndex]);
    DrawText("00.00", gap + MeasureText("Time:", 30), padding, 30, RAYWHITE);

    DrawText("Hit:", padding, spacing + padding, 30, targetColors[hudColorIndex]);
    DrawText("0", gap + MeasureText("Hit:", 30), spacing + padding, 30, RAYWHITE);

    DrawText("Acc:", padding, spacing * 2 + padding, 30, targetColors[hudColorIndex]);
    DrawText("0", gap + MeasureText("Acc:", 30), spacing * 2 + padding, 30, RAYWHITE);

}
    
void DrawPauseMenu(void)
{
    int buttonWidth = GetScreenWidth() / 6;
    int buttonHeight = GetScreenHeight() / 12;
    int gap = 20 + (buttonHeight);
    //Fade Overlay
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){ 0, 0, 0, 150 });


    //Button Text Size
    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);

    if (GuiButton((Rectangle){ centerX - buttonWidth / 2, (centerY - buttonHeight / 2) , buttonWidth, buttonHeight }, "Settings"))
        {
            menu = SETTINGS;
        }

        if (GuiButton((Rectangle){ centerX - buttonWidth / 2 , (centerY - buttonHeight / 2) - gap, buttonWidth, buttonHeight }, "Resume"))
        {
            menu = MAIN;
            DisableCursor();
        }

        if (GuiButton((Rectangle){ centerX - buttonWidth / 2, (centerY - buttonHeight / 2) + gap, buttonWidth, buttonHeight }, "Quit"))
        {
            menu = MAIN;
            DisableCursor();     
        }
   
}


void DrawSettingsMenu(void)
{
    // Fade Overlay
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 150});

    // Menu dimensions
    int menuWidth = 600;
    int menuHeight = 450;
    int menuX = centerX - menuWidth / 2;
    int menuY = centerY - menuHeight / 2;


    // Group Box
    GuiSetStyle(DEFAULT, TEXT_SIZE, 40);
    GuiSetStyle(DEFAULT, TEXT_SPACING, 5);
    GuiGroupBox((Rectangle){menuX, menuY, menuWidth, menuHeight}, "Settings");

    int controlWidth = 400;
    int controlHeight = 40;
    int spacingY = 100; 
    int startY = menuY + 60; 

    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    GuiSetStyle(DEFAULT, TEXT_SPACING, 3);

    
    // --------------------
    // Master Volume Slider

    DrawText("Master Volume", centerX - MeasureText("Master Volume", 20)/2, startY, 20, RAYWHITE);
    GuiSliderBar((Rectangle){centerX - controlWidth/2, startY + 30, controlWidth, controlHeight},
                 NULL, TextFormat("%.2f", volume), &volume, 0.0f, 1.0f);

    // --------------------
    // Mouse Sensitivity Slider
    int sensY = startY + spacingY;
    DrawText("Mouse Sensitivity", centerX - MeasureText("Mouse Sensitivity", 20)/2, sensY, 20, RAYWHITE);
    GuiSliderBar((Rectangle){centerX - controlWidth/2, sensY + 30, controlWidth, controlHeight},
                 NULL, TextFormat("%.2f", sensitivity), &sensitivity, 0.1f, 3.0f);


    // --------------------
    // Target Color Arrow Picker
    int colorY = sensY + spacingY;
    DrawText("Target Color", centerX - MeasureText("Target Color", 20)/2, colorY, 20, RAYWHITE);

    if (GuiButton((Rectangle){centerX - controlWidth / 2 - 50, colorY + 30, 40, controlHeight}, "<"))
    {
        colorIndex--;
        if (colorIndex < 0) colorIndex = 6;
    }
    if (GuiButton((Rectangle){centerX + controlWidth / 2 + 10, colorY + 30, 40, controlHeight}, ">"))
    {
        colorIndex++;
        if (colorIndex > 6) colorIndex = 0;
    }

    // Color preview rectangle
    DrawRectangle(centerX - controlWidth / 2, colorY + 30, controlWidth, controlHeight, targetColors[colorIndex]);
    DrawText(colorNames[colorIndex], centerX - MeasureText(colorNames[colorIndex], 20)/2, colorY + 40, 20, BLACK);

    // --------------------
    // Back button
    if (GuiButton((Rectangle){centerX - 100, menuY + menuHeight - 70, 200, 50}, "Back"))
    {
        menu = PAUSE;
    }
}

void UpdateGlobals(void)
{
    centerX = GetScreenWidth() / 2;
    centerY = GetScreenHeight() / 2;
}

void InitTheme(const char *theme)
{
    if (strcmp(theme, "DARK") == 0)
    {
        GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt((Color){20, 20, 20, 255}));
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt((Color){40, 40, 40, 255}));
        GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt((Color){60, 60, 60, 255}));
        GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt((Color){90, 90, 90, 255}));
        GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt((Color){220, 220, 220, 255}));
    }
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

    float dx = delta.x * sensitivity * degPerCount * dpiScale * DEG2RAD;
    float dy = delta.y * sensitivity * degPerCount * dpiScale * DEG2RAD;

    yaw   -= dx;   
    pitch += -dy;  

    float pitchLimit = 89.0f * DEG2RAD;
    if (pitch > pitchLimit) pitch = pitchLimit;
    if (pitch < -pitchLimit) pitch = -pitchLimit;

    Vector3 forward;
    forward.x = cosf(pitch) * sinf(yaw);
    forward.y = sinf(pitch);
    forward.z = cosf(pitch) * cosf(yaw);

    camera.target = Vector3Add(camera.position, forward);
    Vector2 screenCenter = { GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    mouseRay = GetMouseRay(screenCenter, camera);
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

void DrawTargets(void)
{
    for (int i = 0; i < sizeof(targets)/sizeof(targets[0]); i++)
    {
        DrawSphere(targets[i].position, targetSize, targetColors[colorIndex]);
    }
}

int CheckTargetHit(void)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && menu == MAIN)
    {
        for (int i = 0; i < sizeof(targets)/sizeof(targets[0]); i++)
        {
            RayCollision sphereCollision = GetRayCollisionSphere(mouseRay, targets[i].position, targetSize);

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


//
//----------------------------------------------------------------------------------
// Game Engine
//----------------------------------------------------------------------------------
//



void initEngine(void) 
{
    gameEngine.time = 999.0f;
    gameEngine.sphereSize = 0.5f;
    gameEngine.targetCount = 3;
    gameEngine.gap = 10;
    gameEngine.targetHealth = 1;

    gameEngine.minZ = 100;
    gameEngine.maxZ = 100;

    gameEngine.xVar = 100;
    gameEngine.yVar = 100;

    targets = malloc(gameEngine.targetCount * sizeof(Target));
    gameEngine.Update = GridShot;

}

float Vector3Distance(Vector3 a, Vector3 b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dz = a.z - b.z;
    return sqrtf(dx*dx + dy*dy + dz*dz);
}

int checkInterference(Vector3 pos)
{
    int n = gameEngine.targetCount;
    for (int i = 0; i < n; i++)
    {
        if (Vector3Distance < gameEngine.gap)
        {
            return 1;
        }
    }
    return 0;

}

//----------------------------------------------------------------------------------
// Grid Shot
//----------------------------------------------------------------------------------
void GridShot(void) 
{
    int n = gameEngine.targetCount;
    int maxFreq = 50;

    for (int i = 0; i < n; i++)
    {
        if (targets[i].health <= 0)
        {
            while (maxFreq > 0){
                //Attempt to create Target (maxFreq = 50)
                int x = (rand() % (gameEngine.xVar / 10));
                int y = (rand() % (gameEngine.yVar / 10));
                int z = ((rand() % (gameEngine.maxZ / 10)) + gameEngine.minZ );
                Vector3 pos = {x,y,z};

                





                maxFreq -= 1;
            }


        }
    }
}