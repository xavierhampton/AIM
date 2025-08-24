#include  "common.h"

//----------------------------------------------------------------------------------
// Function Predefs
//----------------------------------------------------------------------------------

int checkInterference(Vector3 pos);


//----------------------------------------------------------------------------------
// Grid Shot
//----------------------------------------------------------------------------------
void Gridshot(void) 
{
    int n = targetEngine.targetCount;
    int maxFreq = 50;

    for (int i = 0; i < n; i++)
    {
        if (targets[i].health <= 0)
        {
            while (maxFreq > 0){
                //Attempt to create Target (maxFreq = 50)
                int x = (rand() % (targetEngine.xVar / 10)) - (int)(targetEngine.xVar / (2 * targetEngine.gap));
                int y = (rand() % (targetEngine.yVar / 10)) + 1;
                int z = targetEngine.maxZ / 10;

                Vector3 pos = {x,y,z};

                if (!checkInterference(pos)) {
                    Target newTarget = {pos, targetEngine.targetHealth};
                    targets[i] = newTarget;
                    break;
                }
                maxFreq -= 1;
                if (maxFreq == 0) {perror("Not Enough Space for Targets... Aborting");}
            }


        }
    }
}

//----------------------------------------------------------------------------------
// Track Shot
//----------------------------------------------------------------------------------
static int dir = 1;
void Track(void) 
{
    if (targets[0].health <= 0) 
    {
        Vector3 pos = {0, targetEngine.yVar, targetEngine.maxZ};
        targets[0] = (Target){pos, targetEngine.targetHealth};
    }
    Target old = targets[0];
    float delta = GetFrameTime();
    

    for (int i = 0; i < n; i++)
    {
        if (targets[i].health <= 0)
        {
            while (maxFreq > 0){
                //Attempt to create Target (maxFreq = 50)
                int x = (rand() % (targetEngine.xVar / 10)) - (int)(targetEngine.xVar / (2 * targetEngine.gap));
                int y = (rand() % (targetEngine.yVar / 10)) + 1;
                int z = targetEngine.maxZ / 10;

                Vector3 pos = {x,y,z};

                if (!checkInterference(pos)) {
                    Target newTarget = {pos, targetEngine.targetHealth};
                    targets[i] = newTarget;
                    break;
                }
                maxFreq -= 1;
                if (maxFreq == 0) {perror("Not Enough Space for Targets... Aborting");}
            }


        }
    }
}

//----------------------------------------------------------------------------------
// X Shot
//----------------------------------------------------------------------------------




//----------------------------------------------------------------------------------
// Helpers
//----------------------------------------------------------------------------------
int checkInterference(Vector3 pos)
{
    int n = targetEngine.targetCount;
    for (int i = 0; i < n; i++)
    {
        if (Vector3Distance(pos, targets[i].position) < (targetEngine.gap / 10))
        {
            return 1;
        }
    }
    return 0;

}