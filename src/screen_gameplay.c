
#define MAX_COLUMNS 20

#include "raylib.h"
#include "rcamera.h"
#include "screens.h"
#include <stdio.h>
#include <stdlib.h>

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static Camera camera = { 0 };
Texture2D gridTexture;
static int finishScreen = 0; 

Model model;


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

    gridTexture = LoadTexture("src/resources/textures/texture1.png");
    if (gridTexture.id == 0)
    {
        printf("Failed to load grid texture!\n");
        exit(EXIT_FAILURE);
    }

    // After loading your texture:
    GenTextureMipmaps(&gridTexture);
    SetTextureFilter(gridTexture, TEXTURE_FILTER_ANISOTROPIC_16X);
    SetTextureWrap(gridTexture, TEXTURE_WRAP_REPEAT);

    ToggleFullscreen();

    model = LoadModel("src/resources/models/shootingGallery.obj");
    if (model.meshCount == 0)
    {
        printf("Failed to load model!\n");
        exit(EXIT_FAILURE);
    }
    Texture2D texture = LoadTexture("src/resources/models/text.png"); // Load model texture
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;            // Set map diffuse texture

    DisableCursor();
    SetTargetFPS(300);              
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // TODO: Update GAMEPLAY screen variables here!
    UpdateCamera(&camera, CAMERA_FIRST_PERSON);


    if (IsKeyPressed(KEY_F11))
    {
        ToggleBorderlessWindowed();
    }

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
    
    int segments = 16;
    Mesh planeMesh = GenMeshPlane(size.x, size.y, segments, segments);

    int vertexCount = (segments + 1) * (segments + 1);

    float *texcoords = planeMesh.texcoords;

    for (int i = 0; i < vertexCount; i++)
    {
        texcoords[i*2] *= 16.0f;     // U
        texcoords[i*2 + 1] *= 16.0f; // V
    }

    // Update texcoords buffer on GPU — pass pointer and offset 0
    UpdateMeshBuffer(planeMesh, 1, texcoords, vertexCount * 2 * sizeof(float), 0);

    Model gridModel = LoadModelFromMesh(planeMesh);
    gridModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = gridTexture;

    DrawModelEx(gridModel, position, (Vector3){1.0f, 0.0f, 0.0f}, rotationXDegrees * DEG2RAD, (Vector3){1.0f, 1.0f, 1.0f}, WHITE);

    UnloadModel(gridModel);
}



void DrawMap(void)
{
    BeginMode3D(camera);

    // Draw the grid
    DrawGrid(20, 2.0f);
    DrawModel(model, (Vector3){0, 0, 0}, 1.0f, WHITE);

    DrawCubeWires((Vector3){0, 0, 0}, 1.0f, 0.01f, 1.0f, RED);

    EndMode3D();
}