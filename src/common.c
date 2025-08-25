#include "common.h"
////////////////
// GLOBAL INITALIZATION
////////////////


Target* targets = NULL;
TargetEngine targetEngine = {0};
GameEngine gameEngine = { 0 };
TargetEngine engines[MAX_ENGINES] = { 0 };
int engineCount = 0;

int menu = MAIN;  
