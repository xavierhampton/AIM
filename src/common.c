#include "common.h"
////////////////
// GLOBAL INITALIZATION
////////////////


Target* targets = NULL;
TargetEngine targetEngine = {0};
GameEngine gameEngine = { 0 };
TargetEngine engines[MAX_ENGINES] = { 0 };
Vector3 adjVelos[MAX_ENGINES] = { 0 };

int engineCount = 0;
float yaw = 0.0;
float pitch = 0.0;

int menu = MAIN;  
