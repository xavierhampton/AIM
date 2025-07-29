#include "Game.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main() {

    Game game(800, 600, "Aim Trainer Lite");
    game.run();
   
    return 0;
}