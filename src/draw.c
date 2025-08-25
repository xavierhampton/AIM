#include "common.h"
#include "raygui.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static Color targetColors[] = { {210, 210, 0, 255}, RED, GREEN, BLUE, YELLOW, ORANGE, PURPLE, RAYWHITE };
static const char *colorNames[] = {"Dark Yellow", "Red", "Green", "Blue", "Yellow", "Orange", "Purple", "White" };

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
void DrawMapSelector(void);
void DrawMapInfo(void);
void DrawSettingsMenu(void);
void InitTheme(const char*);

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
    DrawRectangle(centerX - size, centerY - thickness / 2, size * 2, thickness, targetColors[gameEngine.crosshairColorIndex]); // horizontal bar
    DrawRectangle(centerX - thickness / 2, centerY - size, thickness, size * 2, targetColors[gameEngine.crosshairColorIndex]); // vertical bar
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
    int arrowTexWidth = 256;
    int arrowTexHeight = 64;
    static Texture2D arrowsTex = {0};
    if (arrowsTex.id == 0) {
        arrowsTex = LoadTexture("resources/textures/arrows03.png");
        arrowTexWidth = arrowsTex.width;
        arrowTexHeight = arrowsTex.height;
    } else {
        arrowTexWidth = arrowsTex.width;
        arrowTexHeight = arrowsTex.height;
    }

    // Big Arrow Texture
    int texX = GetScreenWidth() / 2 - arrowTexWidth / 2;
    int texY = 20;
    DrawTexture(arrowsTex, texX, texY, WHITE);

    // Timer Text
    char timerText[16];
    snprintf(timerText, sizeof(timerText), "%.2f", gameEngine.timer);
    int timerFontSize = 46;
    int timerTextWidth = MeasureText(timerText, timerFontSize);
    int timerTextX = GetScreenWidth() / 2 - timerTextWidth / 2;
    int timerTextY = texY + arrowTexHeight / 2 - timerFontSize / 2;
    DrawText(timerText, timerTextX, timerTextY, timerFontSize, targetColors[gameEngine.hudColorIndex]);

    // Small Arrow Textures for Hits/Accuracy
    float smallScale = 0.5f;
    int smallArrowWidth = (int)(arrowTexWidth * smallScale);
    int smallArrowHeight = (int)(arrowTexHeight * smallScale);
    int hitsArrowX = GetScreenWidth() / 2 - smallArrowWidth - 20 - (arrowTexWidth / 2);
    int accArrowX = GetScreenWidth() / 2 + 20 + (arrowTexWidth / 2);
    int arrowsY = texY + 2;

    DrawTextureEx(arrowsTex, (Vector2){hitsArrowX, arrowsY}, 0.0f, smallScale, WHITE);
    DrawTextureEx(arrowsTex, (Vector2){accArrowX, arrowsY}, 0.0f, smallScale, WHITE);

    //Hits Text
    char hitsText[16];
    snprintf(hitsText, sizeof(hitsText), "%d", gameEngine.hits);
    int hitsFontSize = 30;
    int hitsTextWidth = MeasureText(hitsText, hitsFontSize);
    int hitsTextX = hitsArrowX + smallArrowWidth / 2 - hitsTextWidth / 2;
    int hitsTextY =  2 + arrowsY + smallArrowHeight / 2 - hitsFontSize / 2;
    DrawText(hitsText, hitsTextX, hitsTextY, hitsFontSize, targetColors[gameEngine.hudColorIndex]);

    //Accuracy Text
    char accText[16];
    float accuracy = (gameEngine.shots > 0) ? (100.0f * (float)gameEngine.hits / (float)gameEngine.shots) : 0.0f;
    snprintf(accText, sizeof(accText), "%.2f", accuracy);
    int accFontSize = 24;
    int accTextWidth = MeasureText(accText, accFontSize);
    int accTextX = accArrowX + smallArrowWidth / 2 - accTextWidth / 2;
    int accTextY =  2 + arrowsY + smallArrowHeight / 2 - accFontSize / 2;
    DrawText(accText, accTextX, accTextY, accFontSize, targetColors[gameEngine.hudColorIndex]);


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

    DrawMapSelector();
    DrawMapInfo();
   
}


void DrawSettingsMenu(void)
{

    // Fade Overlay
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 150});

    // Menu dimensions
    int menuWidth = 600;
    int menuHeight = 650;
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
        gameEngine.colorIndex--;
        if (gameEngine.colorIndex < 0) gameEngine.colorIndex = 7;
    }
    if (GuiButton((Rectangle){centerX + controlWidth / 2 + 10, colorY + 30, 40, controlHeight}, ">"))
    {
        gameEngine.colorIndex++;
        if (gameEngine.colorIndex > 7) gameEngine.colorIndex = 0;
    }

    // Color preview rectangle
    DrawRectangle(centerX - controlWidth / 2, colorY + 30, controlWidth, controlHeight, targetColors[gameEngine.colorIndex]);
    DrawText(colorNames[gameEngine.colorIndex], centerX - MeasureText(colorNames[gameEngine.colorIndex], 20)/2, colorY + 40, 20, BLACK);

    // --------------------
    // Crosshair Color Arrow Picker
    int crosshairColorY = colorY + spacingY;
    DrawText("Crosshair Color", centerX - MeasureText("Crosshair Color", 20)/2, crosshairColorY, 20, RAYWHITE);

    if (GuiButton((Rectangle){centerX - controlWidth / 2 - 50, crosshairColorY + 30, 40, controlHeight}, "<"))
    {
        gameEngine.crosshairColorIndex--;
        if (gameEngine.crosshairColorIndex < 0) gameEngine.crosshairColorIndex = 7;
    }
    if (GuiButton((Rectangle){centerX + controlWidth / 2 + 10, crosshairColorY + 30, 40, controlHeight}, ">"))
    {
        gameEngine.crosshairColorIndex++;
        if (gameEngine.crosshairColorIndex > 7) gameEngine.crosshairColorIndex = 0;
    }

    // Crosshair color preview rectangle
    DrawRectangle(centerX - controlWidth / 2, crosshairColorY + 30, controlWidth, controlHeight, targetColors[gameEngine.crosshairColorIndex]);
    DrawText(colorNames[gameEngine.crosshairColorIndex], centerX - MeasureText(colorNames[gameEngine.crosshairColorIndex], 20)/2, crosshairColorY + 40, 20, BLACK);

    // --------------------
    // HUD Color Arrow Picker
    int hudColorY = crosshairColorY + spacingY;
    DrawText("HUD Color", centerX - MeasureText("HUD Color", 20)/2, hudColorY, 20, RAYWHITE);

    if (GuiButton((Rectangle){centerX - controlWidth / 2 - 50, hudColorY + 30, 40, controlHeight}, "<"))
    {
        gameEngine.hudColorIndex--;
        if (gameEngine.hudColorIndex < 0) gameEngine.hudColorIndex = 7;
        InitTheme("DARK");

    }
    if (GuiButton((Rectangle){centerX + controlWidth / 2 + 10, hudColorY + 30, 40, controlHeight}, ">"))
    {
        gameEngine.hudColorIndex++;
        if (gameEngine.hudColorIndex > 7) gameEngine.hudColorIndex = 0;
        InitTheme("DARK");

    }

    // HUD color preview rectangle
    DrawRectangle(centerX - controlWidth / 2, hudColorY + 30, controlWidth, controlHeight, targetColors[gameEngine.hudColorIndex]);
    DrawText(colorNames[gameEngine.hudColorIndex], centerX - MeasureText(colorNames[gameEngine.hudColorIndex], 20)/2, hudColorY + 40, 20, BLACK);


    if (GuiButton((Rectangle){centerX - 100, menuY + menuHeight - 70, 200, 50}, "Back"))
    {
        menu = PAUSE;
        SaveSettings();
    }
}
void DrawMapSelector(void)
{

    static int selectedMap = 0;
    static int scrollIndex = 0;

    const int listWidth   = 500;
    const int listHeight  = 435;
    const int itemHeight  = 60;
    const int visibleCount = 7;
    const int padding      = 10;


    static const char *mapNames[] = {
        "Arena", "Desert", "Forest", "City",
        "Space", "Ruins", "Cave", "Island"
    };
    const int mapCount = sizeof(mapNames) / sizeof(mapNames[0]);


    int x = centerX + listWidth / 2 + (GetScreenHeight() / 12) + 20;
    int y = GetScreenHeight() / 2  - listHeight / 2;


    DrawRectangle(x, y, listWidth, listHeight, (Color){30, 30, 30, 200});


    int wheel = GetMouseWheelMove();
    if (wheel != 0)
    {
        scrollIndex -= wheel;
        if (scrollIndex < 0) scrollIndex = 0;
        if (scrollIndex > mapCount - visibleCount) scrollIndex = mapCount - visibleCount;
        if (scrollIndex < 0) scrollIndex = 0; // handles case where mapCount < visibleCount
    }


    for (int i = 0; i < visibleCount && (i + scrollIndex) < mapCount; i++)
    {
        int itemIndex = i + scrollIndex;
        int itemY = y + padding + i * itemHeight;
        Rectangle itemRect = {x + padding, itemY, listWidth - 3 * padding, itemHeight - 4};

        bool hovered    = CheckCollisionPointRec(GetMousePosition(), itemRect);
        bool isSelected = (selectedMap == itemIndex);

        // Background Color
        Color bgColor = (Color){50, 50, 70, 120};
        if (hovered)    bgColor = (Color){targetColors[gameEngine.hudColorIndex].r / 3, targetColors[gameEngine.hudColorIndex].g / 3, targetColors[gameEngine.hudColorIndex].b / 3, 180};
        if (isSelected) bgColor = (Color){targetColors[gameEngine.hudColorIndex].r, targetColors[gameEngine.hudColorIndex].g, targetColors[gameEngine.hudColorIndex].b, 180};

        DrawRectangleRec(itemRect, bgColor);

        // Text
        DrawText(mapNames[itemIndex], itemRect.x + 12, itemRect.y + 8, 20, RAYWHITE);

        // Selection
        if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            selectedMap = itemIndex;
            // TODO: Apply selected map
        }
    }


    if (mapCount > visibleCount)
    {
        float barHeight = (float)listHeight * visibleCount / mapCount;
        float barY = y + ((float)scrollIndex / (mapCount - visibleCount)) * (listHeight - barHeight);

        DrawRectangle(x + listWidth - 8, y + 2, 6, listHeight - 4, (Color){40, 40, 40, 180});
        DrawRectangle(x + listWidth - 8, barY + 2, 6, barHeight - 4, (Color){targetColors[gameEngine.hudColorIndex].r, targetColors[gameEngine.hudColorIndex].g, targetColors[gameEngine.hudColorIndex].b, 180});
    }
}

void DrawMapInfo(void)
{
    int x = 20;
    int y = 20;
    int lineHeight = 34;
    int fontSize = 30;
    Color textColor = targetColors[gameEngine.hudColorIndex];

    // Display mapType if available (assuming 0 is invalid)
    
    // Display map title if available
    if (targetEngine.title && targetEngine.title[0] != '\0') {
        DrawText(targetEngine.title, x, y, fontSize, textColor);
        y += lineHeight;
    }
    if (targetEngine.mapType != 0) {
        DrawText(TextFormat("Map Type: %d", targetEngine.mapType), x, y, fontSize, textColor);
        y += lineHeight;
    }
    // Display targetSize if non-zero
    if (targetEngine.targetSize != 0.0f) {
        DrawText(TextFormat("Target Size: %.2f", targetEngine.targetSize), x, y, fontSize, textColor);
        y += lineHeight;
    }
    // Display targetCount if non-zero
    if (targetEngine.targetCount != 0) {
        DrawText(TextFormat("Target Count: %d", targetEngine.targetCount), x, y, fontSize, textColor);
        y += lineHeight;
    }
    // Display targetHealth if non-zero
    if (targetEngine.targetHealth != 0) {
        DrawText(TextFormat("Target Health: %d", targetEngine.targetHealth), x, y, fontSize, textColor);
        y += lineHeight;
    }
    // Display targetSpeed if non-zero
    if (targetEngine.targetSpeed != 0) {
        DrawText(TextFormat("Target Speed: %d", targetEngine.targetSpeed), x, y, fontSize, textColor);
        y += lineHeight;
    }
    // Display gap if non-zero
    if (targetEngine.gap != 0.0f) {
        DrawText(TextFormat("Gap: %d", targetEngine.gap), x, y, fontSize, textColor);
        y += lineHeight;
    }
    // Display xVar if non-zero
    if (targetEngine.xVar != 0) {
        DrawText(TextFormat("X Var: %d", targetEngine.xVar), x, y, fontSize, textColor);
        y += lineHeight;
    }
    // Display yVar if non-zero
    if (targetEngine.yVar != 0) {
        DrawText(TextFormat("Y Var: %d", targetEngine.yVar), x, y, fontSize, textColor);
        y += lineHeight;
    }
    // Display minZ if non-zero
    if (targetEngine.minZ != 0) {
        DrawText(TextFormat("Min Z: %d", targetEngine.minZ), x, y, fontSize, textColor);
        y += lineHeight;
    }
    // Display maxZ if non-zero
    if (targetEngine.maxZ != 0) {
        DrawText(TextFormat("Max Z: %d", targetEngine.maxZ), x, y, fontSize, textColor);
        y += lineHeight;
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

    // Normal: gray border, Focused: colored border, Pressed: colored border
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, ColorToInt((Color){120, 120, 120, 255}));
    GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, ColorToInt((Color){targetColors[gameEngine.hudColorIndex].r, targetColors[gameEngine.hudColorIndex].g, targetColors[gameEngine.hudColorIndex].b, 180}));
    GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, ColorToInt(targetColors[gameEngine.hudColorIndex]));
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt((Color){40, 40, 40, 255}));
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt((Color){40, 40, 40, 255}));
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt((Color){60, 60, 60, 255}));
    
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(RAYWHITE));
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt((Color){targetColors[gameEngine.hudColorIndex].r, targetColors[gameEngine.hudColorIndex].g, targetColors[gameEngine.hudColorIndex].b, 180}));
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt((Color){targetColors[gameEngine.hudColorIndex].r, targetColors[gameEngine.hudColorIndex].g, targetColors[gameEngine.hudColorIndex].b, 180}));
}

void DrawTargets(void)
{
    int n = targetEngine.targetCount;
    for (int i = 0; i < n; i++)
    {
        DrawSphere(targets[i].position, targetEngine.targetSize, targetColors[gameEngine.colorIndex]);
    }
}

