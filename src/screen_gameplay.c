
#define MAX_COLUMNS 20

#include "raylib.h"
#include "rcamera.h"
#include "screens.h"
#include <stdio.h>

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static Camera camera = { 0 };
static int finishScreen = 0; // Flag to indicate if the gameplay screen should finish

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------
void DrawCrosshair(void);
void DrawPlaneFlooring(void);

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    camera.position = (Vector3){ 0.0f, 2.0f, 4.0f };
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    DisableCursor();
    SetTargetFPS(300);              
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // TODO: Update GAMEPLAY screen variables here!
    UpdateCamera(&camera, CAMERA_FIRST_PERSON);

    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    DrawPlaneFlooring();
    DrawCrosshair();
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
}

// Gameplay Screen should finish?
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
    DrawRectangle(centerX - size, centerY - thickness / 2, size * 2, thickness, RED); // horizontal bar
    DrawRectangle(centerX - thickness / 2, centerY - size, thickness, size * 2, RED); // vertical bar
}

void DrawPlaneFlooring(void)
{
    BeginMode3D(camera);
    // Draw the plane flooring
    DrawPlane((Vector3){0.0f, 0.0f, 0.0f}, (Vector2){32.0f, 32.0f}, (Color){10, 10, 10, 255});
    float gridSize = 32.0f;
    float gridStep = 2.0f; // spacing between grid lines
    for (float i = -gridSize/2; i <= gridSize/2; i += gridStep)
    {
        DrawLine3D((Vector3){-gridSize/2, 0.01f, i}, (Vector3){gridSize/2, 0.01f, i}, (Color){255, 255, 255, 100});
        DrawLine3D((Vector3){i, 0.01f, -gridSize/2}, (Vector3){i, 0.01f, gridSize/2}, (Color){255, 255, 255, 100});
    }
    EndMode3D();
}