#include "common.h"
#include "raygui.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static Color targetColors[] = { {210, 210, 0, 255}, RED, GREEN, BLUE, YELLOW, ORANGE, PURPLE };
static const char *colorNames[] = {"Dark Yellow", "Red", "Green", "Blue", "Yellow", "Orange", "Purple" };
static int colorIndex = 0;
static int hudColorIndex = 2;

static int centerX = {0};
static int centerY = {0};

Model skybox = { 0 };

//----------------------------------------------------------------------------------
// Function Predefs
//----------------------------------------------------------------------------------
void InitDraw(void);
void UnloadDraw(void);
void DrawCrosshair(void);
void DrawMap(void);
void DrawGUI(void);
void DrawHUD(void);
void DrawPauseMenu(void);
void DrawTargets(void);

void DrawSettingsMenu(void);

void InitDraw(void) {
    skybox = LoadModel("resources/models/skybox.glb");
    assert(skybox.meshCount > 0);

}

void UnloadDraw(void) {
    UnloadModel(skybox);
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
    BeginMode3D(gameEngine.camera);

    //SKYBOX!!!
    rlDisableBackfaceCulling();
    rlDisableDepthMask();
    DrawModel(skybox, (Vector3){0, 0, 0}, 1.0f, WHITE);
    rlEnableDepthMask();
    rlEnableBackfaceCulling();

    //FLOOR
    DrawGrid(12, 0.5f);

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
                 NULL, TextFormat("%.2f", gameEngine.volume), &gameEngine.volume, 0.0f, 1.0f);

    // --------------------
    // Mouse Sensitivity Slider
    int sensY = startY + spacingY;
    DrawText("Mouse Sensitivity", centerX - MeasureText("Mouse Sensitivity", 20)/2, sensY, 20, RAYWHITE);
    GuiSliderBar((Rectangle){centerX - controlWidth/2, sensY + 30, controlWidth, controlHeight},
                 NULL, TextFormat("%.2f", gameEngine.sensitivity), &gameEngine.sensitivity, 0.1f, 3.0f);


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

void DrawTargets(void)
{
    int n = targetEngine.targetCount;
    for (int i = 0; i < n; i++)
    {
        DrawSphere(targets[i].position, targetEngine.sphereSize, targetColors[colorIndex]);
    }
}

