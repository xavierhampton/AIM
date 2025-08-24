#include  "common.h"

void LoadSettings(const char *filename, GameEngine *engine) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open %s\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // remove newline
        line[strcspn(line, "\r\n")] = 0;

        char key[64];
        char value[128];

        if (sscanf(line, "%63[^=]=%127s", key, value) == 2) {
            if (strcmp(key, "sensitivity") == 0) {
                engine->sensitivity = strtof(value, NULL);
            }
            else if (strcmp(key, "volume") == 0) {
                engine->volume = strtof(value, NULL);
            }
           
        }
    }

    fclose(file);
}