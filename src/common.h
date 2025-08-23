
#ifndef COMMON_H
#define COMMON_H

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rcamera.h"
#include "common.h"
#include "assert.h"
#include "rlgl.h"
#include "raymath.h"
#include "math.h"

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum GameScreen { UNKNOWN = -1, LOGO = 0, GAMEPLAY } GameScreen;
enum MENU { MAIN, PAUSE, SETTINGS };

typedef struct target {
    Vector3 position;
    int health;
} Target;

typedef struct target_engine {
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
} TargetEngine;


typedef struct game_engine {
    Camera camera;
    Ray mouseRay;

    float sensitivity;
    float volume;





}GameEngine;


//----------------------------------------------------------------------------------
// Global Variables Declaration (shared by several modules)
//----------------------------------------------------------------------------------
extern GameScreen currentScreen;
extern Font font;
extern Music music;
extern Sound fxCoin;


extern int menu;
extern TargetEngine targetEngine;
extern GameEngine gameEngine;
extern Target* targets;

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Declaration
//----------------------------------------------------------------------------------

//gameplay
void InitGameplayScreen(void);
void UpdateGameplayScreen(void);
void DrawGameplayScreen(void);
void UnloadGameplayScreen(void);
int FinishGameplayScreen(void);

// draw
#ifndef DRAW_H
#define DRAW_H

void InitDraw(void);
void UnloadDraw(void);
void DrawCrosshair(void);
void DrawMap(void);
void DrawGUI(void);
void DrawHUD(void);
void DrawPauseMenu(void);
void DrawSettingsMenu(void);
void DrawTargets(void);
void UpdateGlobals(void);

#endif


#ifdef __cplusplus
}
#endif

#endif