#include  "common.h"
#include <dirent.h>   
#include <sys/types.h>

void LoadMap(const char *filename, TargetEngine *engine);

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

int LoadMaps(const char *dirPath, int maxEngines) {
    DIR *dir = opendir(dirPath);
    if (!dir) {
        perror("opendir failed");
        return 0;
    }

    struct dirent *entry;
    int count = 0;

    while ((entry = readdir(dir)) != NULL && count < maxEngines) {
        if (entry->d_name[0] == '.')
            continue;

        const char *ext = strrchr(entry->d_name, '.');
        if (!ext || strcmp(ext, ".map") != 0)
            continue;

        char path[512];
        snprintf(path, sizeof(path), "%s/%s", dirPath, entry->d_name);

        LoadMap(path, &engines[count]);

        count++;
        printf("AA\n\n");
    }

    closedir(dir);
    return count; // number of files loaded
}

void LoadMap(const char *filename, TargetEngine *engine) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open %s\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0;

        char key[64];
        char value[128];

        if (sscanf(line, "%63[^=]=%127s", key, value) == 2) {
            if (strcmp(key, "time") == 0) {
                engine->time = strtof(value, NULL);
            }
            else if (strcmp(key, "targetSize") == 0) {
                engine->targetSize = strtof(value, NULL);
            }
            else if (strcmp(key, "targetCount") == 0) {
                engine->targetCount = strtol(value, NULL, 10);
            }
            else if (strcmp(key, "targetHealth") == 0) {
                engine->targetHealth = strtol(value, NULL, 10);
            }
            else if (strcmp(key, "gap") == 0) {
                engine->gap = strtol(value, NULL, 10);
            }
            else if (strcmp(key, "xVar") == 0) {
                engine->xVar = strtol(value, NULL, 10);
            }
            else if (strcmp(key, "yVar") == 0) {
                engine->yVar = strtol(value, NULL, 10);
            }
            else if (strcmp(key, "minZ") == 0) {
                engine->minZ = strtol(value, NULL, 10);
            }
            else if (strcmp(key, "maxZ") == 0) {
                engine->maxZ = strtol(value, NULL, 10);
            }
        }
    }

    fclose(file);
}

