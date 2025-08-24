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
        Vector3 pos = {0, targetEngine.yVar/ 10 + 1, targetEngine.maxZ / 10};
        targets[0] = (Target){pos, targetEngine.targetHealth};
    }

    Target old = targets[0];
    float delta = GetFrameTime();
    float speed = targetEngine.targetSpeed / 10;

    if (dir == 1 && old.position.x > (targetEngine.xVar / 10))
    {
        old.position.x = targetEngine.xVar / 10;
        dir = -1;
    }

    else if (dir == -1 && old.position.x < -(targetEngine.xVar / 10))
    {
        old.position.x = -(targetEngine.xVar / 10);
        dir = 1;
    }

    Vector3 pos = (Vector3){old.position.x + (dir * delta * speed), old.position.y, old.position.z};
    printf("dir=%d, delta=%.4f, speed=%.4f\n", dir, delta, speed);
    printf("Target position: x=%.2f, y=%.2f, z=%.2f\n", pos.x, pos.y, pos.z);
    targets[0].position = pos;
    
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