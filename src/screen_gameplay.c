#include "raylib.h"
#include "rcamera.h"
#include "screens.h"
#include "assert.h"
#include <stdio.h>
#include <stdlib.h>
#include "rlgl.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int finishScreen = 0; 
static Camera camera = { 0 };
Model skybox = { 0 };


//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------
void DrawCrosshair(void);
void DrawMap(void);

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    //GLOBAL SETTINGS
    ToggleFullscreen();

    //CAMERA SETTINGS
    camera.position = (Vector3){ 0.0f, 2.0f, 0.0f };
    camera.target = (Vector3){ 0.0f, 2.0f, 1.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 103.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    //MODEL LOADING
    skybox = LoadModel("src/resources/models/skybox.glb");
    assert(skybox.meshCount > 0);


    //GAME START
    DisableCursor();
    SetTargetFPS(300);              
}

void UpdateGameplayScreen(void)
{
    UpdateCamera(&camera, CAMERA_FIRST_PERSON);



    //KEY HOOKS
    if (IsKeyPressed(KEY_F11))
    {
        ToggleBorderlessWindowed();
    }
    if (IsKeyPressed(KEY_ENTER))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }
}


void DrawGameplayScreen(void)
{
    DrawMap();
    DrawCrosshair();
}

void UnloadGameplayScreen(void)
{
    UnloadModel(skybox);
}

int FinishGameplayScreen(void)
{
    return finishScreen;
}

//----------------------------------------------------------------------------------
// Private Functions
//----------------------------------------------------------------------------------
void DrawCrosshair(void)
{
    int centerX = GetScreenWidth() / 2;
    int centerY = GetScreenHeight() / 2;
    int size = 6;  // size of the crosshair arms

    int thickness = 2;
    DrawRectangle(centerX - size, centerY - thickness / 2, size * 2, thickness, WHITE); // horizontal bar
    DrawRectangle(centerX - thickness / 2, centerY - size, thickness, size * 2, WHITE); // vertical bar
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


    DrawGrid(8, 0.5f);

    EndMode3D();
}