
#define MAX_COLUMNS 20

#include "raylib.h"
#include "rcamera.h"
#include "screens.h"
#include <stdio.h>

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static Camera camera = { 0 };
Texture2D gridTexture;
static int finishScreen = 0; 


//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------
void DrawCrosshair(void);
void DrawMap(void);

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    camera.position = (Vector3){ 0.0f, 2.0f, 4.0f };
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    gridTexture = LoadTexture("src/resources/texture1.png");

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
    DrawMap();
    DrawCrosshair();
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    UnloadTexture(gridTexture); // Unload grid texture
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

void DrawGridPlane(Vector3 position, Vector2 size, float rotationXDegrees)
{
    Vector2 texSize = { size.x * 2, size.y * 2 };
    Vector2 texCoord = { 0, 0 };

    // Draw the grid texture
    Mesh planeMesh = GenMeshPlane(32.0f, 32.0f, 1, 1);
    Model gridModel = LoadModelFromMesh(planeMesh);

    gridModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = gridTexture;
    gridModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].value = 8.0f; 
    DrawModel(gridModel, position, 1.0f, WHITE);

    UnloadModel(gridModel);

}

void DrawMap(void)
{
    BeginMode3D(camera);

    // Draw the grid
    DrawGridPlane((Vector3){0, 0, 0}, (Vector2){32.0f, 32.0f}, 0);

    EndMode3D();
}