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



//----------------------------------------------------------------------------------
// Game Variables
//----------------------------------------------------------------------------------
static float sensitivity = 1.0f;
static float volume = 1.0f;

static Color targetColors[] = { {210, 210, 0, 255}, RED, GREEN, BLUE, YELLOW, ORANGE, PURPLE };
static const char *colorNames[] = {"Dark Yellow", "Red", "Green", "Blue", "Yellow", "Orange", "Purple" };
static int colorIndex = 0;

static Vector3 spheres[] = {
    { 0.0f, 1.0f, 6.0f },
    { 4.0f, 1.0f, 6.0f },
    { -4.0f, 1.0f, 6.0f }
};

static float sphereSize = 0.5f;

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
    camera.fovy = 103.0f;
    camera.projection = CAMERA_CUSTOM;
}

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
    int centerX = GetScreenWidth() / 2;
    int centerY = GetScreenHeight() / 2;

    // Draw the HUD elements here
    
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

void UpdateMouse(void)
{

    float sens = sensitivity / 1000;

    Vector2 mouseDelta = GetMouseDelta();

    Matrix rotation = MatrixRotateY(-mouseDelta.x * sens);
    Vector3 forward = Vector3Subtract(camera.target, camera.position);
    forward = Vector3Transform(forward, rotation);

    Vector3 right = Vector3CrossProduct(camera.up, forward);
    rotation = MatrixRotate(right, mouseDelta.y * sens);
    forward = Vector3Transform(forward, rotation);

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
    for (int i = 0; i < sizeof(spheres)/sizeof(spheres[0]); i++)
    {
        DrawSphere(spheres[i], sphereSize, targetColors[colorIndex]);
    }
}

int CheckTargetHit(void)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && menu == MAIN)
    {
        for (int i = 0; i < sizeof(spheres)/sizeof(spheres[0]); i++)
        {
            RayCollision sphereCollision = GetRayCollisionSphere(mouseRay, spheres[i], sphereSize);

            if (sphereCollision.hit)
            {

            PlaySound(fxCoin);
            return i; // Return the index of the hit sphere

            }
        }
    }
    return -1;
}